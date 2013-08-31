#!/usr/bin/perl -w

# server1.pl - a simple server
use warnings;
use strict;
use Socket;
use DBI;

# use port 7890 as default
my $port = shift || 7890;
my $proto = getprotobyname('tcp');

my $dbh = DBI->connect("DBI:Pg:dbname=mydb", "postgres", "postgres", {'RaiseError' => 1});

# create a socket, make it reusable
socket(SERVER, PF_INET, SOCK_STREAM, $proto)
        or die "socket: $!";
setsockopt(SERVER, SOL_SOCKET, SO_REUSEADDR, 1)
        or die "setsock: $!";

# grab a port on this machine
my $paddr = sockaddr_in($port, INADDR_ANY);

# bind to a port, then listen
bind(SERVER, $paddr)      or die "bind: $!";
listen(SERVER, SOMAXCONN) or die "listen: $!";
print "SERVER started on port $port\n";

# for each connection...
my $client_addr;
while ($client_addr = accept(CLIENT, SERVER)) {
	my $data;
	my $card;
	my $cost;
    # find out who connected
    my ($client_port, $client_ip) =
                     sockaddr_in($client_addr);
    my $client_ipnum = inet_ntoa($client_ip);
    my $client_host =
             gethostbyaddr($client_ip, AF_INET);
    # tell who connected
    print "got a connection from: $client_host",
          " [$client_ipnum]\n";
	# send them a message
	#print CLIENT "Hello from the server: \n", $client_ipnum, ":", $client_port, "\n";
    
    # Get data.
    $data = <CLIENT>;
    $data =~ /([^:]+):(.+)\n/;
    $card = $1;
    $cost = $2;
	# data parsed, talk to the database.
	
	print "card is: ",$card,"\n";
	print "cost is: ",$cost,"\n";
	
	my $sth = $dbh->prepare("SELECT id,name,card,saldo FROM rustvaluta.accounts WHERE card=E'\\\\x".$card."'");
	$sth->execute();
	if($sth->rows()==0)
	{
		my $create = 0;
		my $name = "";
		eval {
			local $SIG{ALRM} = sub { die "alarm\n" };
			print "Enter name for new account, or enter to cancel\n";
			print "creation of new account: ";
			alarm 30;
			$name = <>;
			alarm 0;
		};
		$name =~ s/[\n\t\r]//;
		if ($@) {
			die unless $@ eq "alarm\n";
			$create = 1;
			$name = time;
		} else {
			$create = 1 unless $name eq "";
		}
		if ($create)
		{
			$name =~ s/[\n\t\r]//;
			$dbh->do("INSERT INTO rustvaluta.accounts (name,card,saldo) VALUES ('".$name."',E'\\\\x".$card."',0)");
			$sth = $dbh->prepare("SELECT id,name,card,saldo FROM rustvaluta.accounts WHERE card=E'\\\\x".$card."'");
			$sth->execute();
			
		} else {
			print "Creation cancelled.\nclosing connection.\n";
			print CLIENT "0";
			close CLIENT;
		}
			next unless $create;
	}
	
	# print values
	my $ref = $sth->fetchrow_hashref();
	print "Owner of card $card is $ref->{'name'}\n";
	
	
	if($ref->{'saldo'}>=$cost)
	{
		my $rows;
		$rows = $dbh->do("UPDATE rustvaluta.accounts SET saldo=".($ref->{'saldo'}-$cost)."WHERE card=E'\\\\x".$card."'");
		print "Updated account balance.\n" if $rows;
		$rows = $dbh->do("INSERT INTO rustvaluta.transactions (account,change, timestamp ) VALUES(".$ref->{id}.",".-$cost.", NOW() )");
		print "Created transaction.\n" if $rows;
		print CLIENT "1";
	} else {
		print CLIENT "0";
		print "Attempted to draw ".$cost."¤ from account ".$card." (".$ref->{'id'}.":".$ref->{'name'}.") with ".$ref->{'saldo'}."¤ available.\n";
	}
	
	
    # close connection
    close CLIENT;
}
