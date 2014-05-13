#!/usr/bin/perl -w
#
use strict;
use warnings;
use Crypt::RandPasswd;


#program version
my $VERSION="0.1";

#For CVS , use following line
#my $VERSION=sprintf("%d.%02d", q$Revision: 370592 $ =~ /(\d+)\.(\d+)/);

sub do_something {
  my $text=shift;
  print "$text\n";
}

sub find_best_program {

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
my $command = " ";
my $home = " ";
my $programa = " ";
my $program_entry = " ";
my $joao = " ";
my $garbage = " ";
my $available_used = -1;
my $hardrive;
my $tokill;
my $hardrive_command;
my $state=1;
my @args=" ";
my @list;


my $FORMAT = "%-6s %s %s %s\n";
while (1==1) {

sleep 1;
   print "  total     $cpu->{total}\n";
   print "  usage      $memory_percentage \n";
my $stat = $lxs->get;
my $t = new Proc::ProcessTable;
my $cpu  = $stat->cpustats->{cpu};
my $memory_percentage = $stat->memstats->{memusedper};
$hardrive_command = `df -h | grep memory`;
($garbage ,$garbage, $garbage , $garbage , $hardrive) = split / +|	/, $hardrive_command;
print "  $hardrive_command and $hardrive \n";
$hardrive =~ s/%//g;
$hardrive = sprintf("%.f",$hardrive);


$tokill = `cat /home/joao/tokill`;
$garbage = `rm /home/joao/tokill`;
$tokill = sprintf("%d",$tokill);

   if ($state == 0 ) {
	$tokill = `cat /home/joao/location_ultimo.txt`;
	$garbage = `rm /home/joao/location_ultimo.txt`;
	$tokill = sprintf("%d",$tokill);
    }



if ($cpu->{total} > 80 ) {
print " CPU to high hopefully ofender will be killed  $cpu->{total} \n";
} else {
print " CPU winthin acceptable limits $cpu->{total}\%used\n";
}

if ($memory_percentage > 90 ) {
	print " Memory to high hopefully ofender will be killed \n";
} else {
	print " Memorty winthin acceptable $memory_percentage\%used \n";
}

if ($hardrive > 90 ) {
	print " hardrive to high hopefully ofender will be killed $hardrive  \n";
} else {
	print " hardrive winthin acceptable $hardrive \n";
}

if ($cpu->{total} < 60 && $memory_percentage < 80 && $hardrive < 90) {
# CPU and Memory in acceptable ranges, create and launch new program
	$state =1 ;
} else {
	print " no conditions to make more childs\n";
	$state =0 ;
}



  open FILE, "/home/joao/location.txt";
  # read the record, and chomp off the newline
  chomp(my $best_program = <FILE>);
  close FILE;
  open FILE, "/home/joao/available.txt";
  # read the record, and chomp off the newline
  chomp(my $available = <FILE>);
  close FILE;
foreach my $p ( @{$t->table}) {
   if ( $p->cmndline =~ "/home/joao/memory") {
	my $commandline = $p->cmndline ;
	chomp($commandline);
	($command,$program_entry) = split / /, $commandline ;
	$program_entry = sprintf("%d", $program_entry);
	if ( $program_entry == $best_program && $available_used != $available && $state == 1 ) {
		($garbage,$home,$joao,$garbage,$programa) = split /\//, $command;
		my $new_word = Crypt::RandPasswd->letters( 7, 7 );
		print ( " o novo vai ser $new_word na posicato $available \n");
		system("/home/joao/make_child/debug/src/make_child $new_word $programa");
		system("chmod 770 /home/joao/memory/$new_word* ");
		system("nohup /home/joao/memory/$new_word $available > /dev/null &");
		$available_used = $available;
	}
	if ($program_entry == $tokill) {
		($garbage,$home,$joao,$garbage,$programa) = split /\//, $command;
		@args = ("kill", $p->pid);
   		system(@args) == 0
                or print "system @args failed: $?";
		$garbage = `rm /home/joao/memory/$programa`;
	}
   }

   if ( $p->pctmem > 10 || $p->pctcpu > 10) {

          printf($FORMAT, 
          $p->pid, 
	  $p->pctcpu,
	  $p->pctmem,
          $p->cmndline);
	 if ( $p->cmndline =~ "/home/joao/memory" ) {
          @args = ("kill", $p->pid);
   		system(@args) == 0
                or print "system @args failed: $?";
	}
     }
 }
   if (rand() > 0.9999 ) {
	$garbage = `echo " " > /home/joao/cpu_memory_monitoring/nohup.out`;
	print " doing clean up \n ";
	@list = `ls /home/joao/memory/ | sort`;
	foreach $garbage (@list) {
		$home = 0;
		chomp $garbage;
		foreach my $p ( @{$t->table}) {
			#print "$p->cmndline and $garbage \n";
			
			if ( $p->cmndline =~ "/home/joao/memory" && $p->cmndline =~ "$garbage") {
			$home =1;
			} 
		}
		if ($home == 0 ) {
		$joao = `rm /home/joao/memory/$garbage`;
		}
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
