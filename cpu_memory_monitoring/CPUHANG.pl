#!/usr/bin/perl -w
#
use strict;
use warnings;


#program version
my $VERSION="0.1";

#For CVS , use following line
#my $VERSION=sprintf("%d.%02d", q$Revision: 370592 $ =~ /(\d+)\.(\d+)/);

sub do_something {
  my $text=shift;
  print "$text\n";
}

&do_something("Hello World");

   use Sys::Statistics::Linux;
   use Data::Dumper;
   use Proc::ProcessTable;

    my $lxs = Sys::Statistics::Linux->new(
        sysinfo   => 0,
        cpustats  => 1,
        procstats => 0,
        memstats  => 1,
        pgswstats => 0,
        netstats  => 0,
        sockstats => 0,
        diskstats => 0,
        diskusage => 0,
        loadavg   => 0,
        filestats => 0,
        processes => 0,
    );

    sleep 1;
    my $stat = $lxs->get;
    print " \n";


 my $cpu  = $stat->cpustats->{cpu};
 my $memory_percentage = $stat->memstats->{memusedper};
   print "  total     $cpu->{total}\n";
   print "  usage      $memory_percentage \n";

while (1==1) {
my $stat = $lxs->get;
my $cpu  = $stat->cpustats->{cpu};
my $memory_percentage = $stat->memstats->{memusedper};
if ($cpu->{total} > 80 ) {
print " CPU to high start program to kill offender  $cpu->{total} \n";
} else {
print " CPU winthin acceptable limits $cpu->{total}\%used\n";
}

if ($memory_percentage > 60 ) {
	print " Memory to high start program to kill offender \n";
	} else {
	print " Memorty winthin acceptable $memory_percentage\%used \n";
	}


my $t = new Proc::ProcessTable;
my $FORMAT = "%-6s %s %s %s\n";

foreach my $p ( @{$t->table}) {
   if ( $p->pctmem > 10 || $p->pctcpu > 10) {

          printf($FORMAT, 
          $p->pid, 
	  $p->pctcpu,
	  $p->pctmem,
          $p->cmndline);
	 print $p->pid;
          my @args = ("kill", $p->pid);
   		system(@args) == 0
                or print "system @args failed: $?"
     }
 }

}
print "That's all folks for version $VERSION \n";
__END__

=head1 NAME

cpu_memory_monitoring - short description of your program

=head1 SYNOPSIS

 how to use your program

=head1 DESCRIPTION

 long description of your program

=head1 SEE ALSO

 need to know things before somebody uses your program

=head1 AUTHOR

 joao ferreira

=cut
