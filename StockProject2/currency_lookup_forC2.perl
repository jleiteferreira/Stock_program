#!/usr/bin/perl

   use Finance::Quote;
    use Net::Ping::External qw(ping);
   
   
   
  @timeData = localtime(time);
  
 # if ( @timeData[1] == 0 && @timeData[2] == 0 ) {

 $what = ping(host => "www.google.com");
 print $what;
  
  if ( rand() < 0.01 ) {	  
	use Finance::QuoteHist;
   $qhist = Finance::QuoteHist->new
    (
      symbols    => [qw(FXE)],
      start_date => '01/01/2008', # or 'one year ago', see Date::Manip
      end_date   => 'today',
     );
      open FILE_history, ">/home/joao/StockProject2/stockvaluehistory.txt";
	 foreach $row ($qhist->quotes()) {
		($symbol, $date, $open, $high, $low, $close, $volume) = @$row;
		
		print FILE_history "($symbol , $date , $open , $high , $low , $close )\n";; 
	  }
	  close FILE_history;
}
		
		
		my $q = Finance::Quote->new;
		 my $conversion_rate = $q->currency("EUR","USD");
		 open FILE, ">/home/joao/StockProject2/stockvalue.txt";
		 		print "current value $conversion_rate";
		 print FILE $conversion_rate; 
		 close FILE;
		 
		 
sleep 10;
exec ('perl /home/joao/StockProject2/currency_lookup_forC.perl');
