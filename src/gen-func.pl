#!/usr/bin/perl

# See LICENSE file for copyright and license details.

use strict;
use warnings;
do "./jlib.pl";

sub usage {
	die('Usage: ./' . $0 . ' <file>') if ($#ARGV == -1);
}

my $fname             = $ARGV[0];
my $ATTR_INLINE       = 'JSTR_INLINE';
my $ATTR_RESTRICT     = 'JSTR_RESTRICT';
my $ATTR_DEFAULT      = 'JSTR_FUNC';
my $ATTR_DEFAULT_VOID = 'JSTR_FUNC_VOID';
my $ATTR_RET_NONNULL  = 'JSTR_FUNC_RET_NONNULL';
my $JSTRING           = 'jstr_ty';
my $JSTRING_LIST      = 'jstr_l_ty';
my $VAR_JSTRING       = 'j';
my $VAR_JSTRING_LIST  = 'l';
my $DATA              = 'data';
my $SIZE              = 'size';
my $CAP               = 'capacity';
my $VAR_DATA          = 's';
my $VAR_SIZE          = 'sz';
my $VAR_CAP           = 'cap';
my $J_PREFIX          = '_j';

my $LEN_PREFIX = '_len';

sub add_inline {
	my ($attr_str_ref, $inline_attr_ref) = @_;
	$$attr_str_ref .= "\n" . $$inline_attr_ref
	  if (index($$attr_str_ref, $$inline_attr_ref) == -1);
}

sub rm_nonnull {
	my ($attr_str_ref, $ret_nonnull_attr_ref) = @_;
	$$attr_str_ref =~ s/$$ret_nonnull_attr_ref//;
	$$attr_str_ref =~ s/\n\n\n//;
}

sub grepped {
	my ($arr_ref, $str_ref) = @_;
	foreach (@$arr_ref) {
		return 1 if (index($_, $$str_ref) != -1);
	}
	return 0;
}

usage();

my $file_str1     = jl_file_get_str(\$fname);
my @ignore_prefix = ("PJSTR", "pjstr", "JSTR", "jstr");
jl_file_namespace_macros(\$file_str1, \"PJSTR_", \@ignore_prefix);
my $file_str2 = '';
my @func_arr;

# generate non *_len() functions
foreach (jl_file_to_blocks(\$file_str1)) {
	$file_str2 .= "$_\n\n";
	my ($attr, $rettype, $name, @arg, $body);
	next
	  if ( !jl_fn_get(\$_, \$attr, \$rettype, \$name, \@arg, undef)
		|| $name !~ /^jstr_/
		|| grepped(\@arg, \"...")
		|| scalar(@arg) == 0);
	push(@func_arr, $name);
	next if ($name !~ /$LEN_PREFIX/o
		|| $name =~ /$J_PREFIX$/o);
	my $base_name = $name;
	$base_name =~ s/$LEN_PREFIX(_|$)/$1/o;
	next if (index($file_str1, "$base_name(") != -1);
	add_inline(\$attr, \$ATTR_INLINE);
	$body = (($rettype eq 'void') ? '' : 'return ') . $name . '(';
	$name = $base_name;
	for (my $i = 0; $i < scalar(@arg); ++$i) {
		my $var = jl_arg_get_var(\$arg[$i]);
		if (index($arg[$i], 'size_t') != -1) {
			my $var_str;
			if ($var =~ /$LEN_PREFIX$/o) {
				my $base_var = $var;
				$base_var =~ s/$LEN_PREFIX(_|$)$/$1/o;
				my $i_str = jl_arg_index(\@arg, \$base_var);
				if ($i_str != -1) {
					$var_str = jl_arg_get_var(\$arg[$i_str]);
				} else {
					goto DO_NOTHING;
				}
			} elsif ($var =~ /^[^*]$VAR_SIZE$/o) {
				$var_str = jl_arg_get_var(\$arg[0]);
			} else {
				goto DO_NOTHING;
			}
			$body .= "strlen((char *)$var_str)";
			splice(@arg, $i--, 1);
		} else {
		  DO_NOTHING:
			$body .= $var;
		}
		$body .= ', ';
	}
	$body =~ s/, $//;
	$body .= ");";
	$file_str2 .=
	  jl_fn_to_string(\$attr, \$rettype, \$name, \@arg, \$body) . "\n\n";
	push(@func_arr, $name);
}

my $file_str3 = '';

# generate *_j() functions
foreach (jl_file_to_blocks(\$file_str2)) {
	$file_str3 .= $_ . "\n\n";
	my ($attr, $rettype, $name, @arg, $body);
	next
	  if ( !jl_fn_get(\$_, \$attr, \$rettype, \$name, \@arg, undef)
		|| $name !~ /^jstr_/
		|| $name =~ /$J_PREFIX$/o
		|| scalar(@arg) == 0
		|| index($name, 'unsafe') != -1);
	my $has_size_or_cap = 0;
	my $has_variadic    = 0;
	foreach (@arg) {
		if ($_ =~ /$VAR_SIZE$/ || $_ =~ /$VAR_CAP$/) {
			$has_size_or_cap = 1;
		} elsif (index($_, '...') != -1) {
			$has_variadic = 1;
			last;
		}
	}
	next if ($has_size_or_cap == 0 || $has_variadic);
	my $j_name = $name . $J_PREFIX;
	next if (grepped(\@func_arr, \($j_name)));
	my $returns_end_ptr = 0;
	if ($rettype eq 'void') {
		$body = '';
	} else {
		if ($name =~ /_p(?:_|$)/) {
			$returns_end_ptr = 1;
			$body            = "$VAR_JSTRING->$SIZE = ";
			$rettype         = 'void';
			$attr =~ s/$ATTR_DEFAULT(\W|$)/$ATTR_DEFAULT_VOID$1/o;
		} elsif (grepped(\@func_arr, \($name . '_p'))) {
			next;
		} else {
			$body = 'return ';
		}
	}
	$body .= $name . '(';
	$name = $j_name;
	$name =~ s/_p(_|$)/$1/;
	my $VAR_STRUCT;
	for (my $i = 0; $i < scalar(@arg); ++$i) {
		my $var = jl_arg_get_var(\$arg[$i]);
		if ($i == 0) {
			my $deref = (jl_arg_is_ptr_ptr(\$arg[$i]) ? '&' : '');
			my $STRUCT;
			if (index($arg[0], $JSTRING_LIST) != -1) {
				$STRUCT = $JSTRING_LIST;
				$body .= "$deref$VAR_JSTRING";
				$VAR_STRUCT = $VAR_JSTRING_LIST;
			} else {
				$STRUCT = $JSTRING;
				$body .= "$deref$VAR_JSTRING->$DATA";
				$VAR_STRUCT = $VAR_JSTRING;
			}
			$arg[$i] = jl_arg_is_const(\$arg[$i]) ? 'const ' : '';
			$arg[$i] .= "$STRUCT *$ATTR_RESTRICT j";
		} elsif ($var eq $VAR_SIZE) {
			my $deref = (jl_arg_is_ptr(\$arg[$i]) ? '&' : '');
			$body .= "$deref$VAR_STRUCT->$SIZE";
			splice(@arg, $i--, 1);
		} elsif ($var eq $VAR_CAP) {
			my $deref = (jl_arg_is_ptr(\$arg[$i]) ? '&' : '');
			$body .= "$deref$VAR_STRUCT->$CAP";
			splice(@arg, $i--, 1);
		} else {
			$body .= $var;
		}
		$body .= ', ';
	}
	$body =~ s/, $//;
	$body .= ')';
	$body .= " - $VAR_STRUCT->$DATA" if ($returns_end_ptr);
	$body .= ";";
	rm_nonnull(\$attr, \$ATTR_RET_NONNULL);
	add_inline(\$attr, \$ATTR_INLINE);
	$file_str3 .=
	  jl_fn_to_string(\$attr, \$rettype, \$name, \@arg, \$body) . "\n\n";
	push(@func_arr, $name);
}

$file_str3 =~ s/\n\n*$/\n/;
print($file_str3);
