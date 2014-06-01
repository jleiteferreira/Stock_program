#!/usr/bin/perl

   use Finance::Quote;
    use Net::Ping::External qw(ping);
   
   
   
  @timeData = localtime(time);
  
 # if ( @timeData[1] == 0 && @timeData[2] == 0 ) {

 $what = ping(host => "www.google.com");
 print $what;
  
if ( ping(host => "www.google.com"))	{ 
   print "internet alive \n";
  if ( rand() < 0.01 ) {	  
	use Finance::QuoteHist;
   $qhist = Finance::QuoteHist->new
    (
      symbols    => [qw(FXE)],
      start_date => '01/01/2003', # or 'one year ago', see Date::Manip
      end_date   => 'today',
     );
      open FILE_history, ">/home/joao/stockvalue/stockvaluehistory.txt";
	 foreach $row ($qhist->quotes()) {
		($symbol, $date, $open, $high, $low, $close, $volume) = @$row;
		
		print FILE_history "($symbol , $date , $open , $high , $low , $close )\n";; 
	  }
	  close FILE_history;
}
		
		
		my $q = Finance::Quote->new;
		$q->failover(1);
		 my $conversion_rate = $q->currency("EUR","USD");
		 open FILE, ">/home/joao/stockvalue/stockvalue.txt";
		if ( $conversion_rate != "" ) {
		 		print "current value $conversion_rate";
		 print FILE $conversion_rate; 
		 close FILE;
		 my $conversion_rate = $q->currency("EUR");
		 print "current value $conversion_rate";
	        } else {
			use Finance::YahooQuote;
			# setting TIMEOUT and PROXY is optional
			my $symbol = "EURUSD=X";
			my @quote;
			@quote = getonequote $symbol; # Get a quote for a single symbol
			my $conversion_rate = $quote[2];
		 	print "current value $conversion_rate";
		 	print FILE $conversion_rate; 
			close FILE;
		}

}		 
sleep 10;
exec ('perl /home/joao/StockProject2/currency_lookup_forC.perl');
