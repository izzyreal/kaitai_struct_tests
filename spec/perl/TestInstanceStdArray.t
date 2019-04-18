# Autogenerated from KST: please remove this line if doing any edits by hand!

package spec::perl::TestInstanceStdArray;

use strict;
use warnings;
use base qw(Test::Class);
use Test::More;
use InstanceStdArray;

sub test_instance_std_array: Test(7) {
    my $r = InstanceStdArray->from_file('src/instance_std_array.bin');

    is($r->ofs(), 16, 'Equals');
    is($r->qty_entries(), 3, 'Equals');
    is($r->entry_size(), 4, 'Equals');
    is(scalar(@{$r->entries()}), 3, 'Equals');
    is(@{$r->entries()}[0], pack('C*', (17, 17, 17, 17)), 'Equals');
    is(@{$r->entries()}[1], pack('C*', (34, 34, 34, 34)), 'Equals');
    is(@{$r->entries()}[2], pack('C*', (51, 51, 51, 51)), 'Equals');
}

Test::Class->runtests;
