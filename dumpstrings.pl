#!/usr/bin/perl
#
# This program analyzes the strings directory and dumps out a HTML
# page with information about those strings.
#
use CGI qw/-no_debug :standard/;

sub PrintTime {
    my($RetTime, $StartTimeArg, $sec, $minute, $hour, $mday, $mon, $year);
    
    ($sec,$minute,$hour,$mday,$mon,$year) = localtime();
    $mon++; # month is 0 based.
    
    printf("%02d/%02d/%02d %02d:%02d:00", $mon,$mday,1900+$year,$hour,$minute );
}

## en-US is in a different file in a different format
my $lang = 'en-US';
foreach my $file (qw(./src/wp/ap/xp/ap_String_Id.h ./src/af/xap/xp/xap_String_Id.h)) {
  open(STRINGS, "< $file" )
    or die "Cannot open $file";
  
  while (<STRINGS>) {
    next unless /dcl\((.*)\s*,\s*\"(.*)\"/;
    my ($dlg,$string) = ($1,$2);
    $string =~ s/&amp/&/;
    $dlgs{$dlg}{$lang} = $string;
    $longest{$dlg} = $lang
  }
  close(STRINGS);
}

## Read in each of the other language files 
## and process them apropriatly
$stringsdir = "./user/wp/strings";
my @lang;
if(scalar @ARGV) {
  @lang = @ARGV;
} else {
  opendir(DIR, $stringsdir) || die "can't opendir $stringsdir: $!";
  @lang = grep { s/\.strings//  } readdir(DIR);
  closedir DIR;
}

#$stringsdir = "./user/wp/strings";
#opendir(DIR, $stringsdir) || die "can't opendir $stringsdir: $!";
#my @lang = grep { s/\.strings//  } readdir(DIR);
#closedir DIR;

 foreach my $lang (@lang) {
  open(STRINGS, "< $stringsdir/$lang.strings") 
    or die "Cannot open $stringsdir/$lang.strings";

  $missing{$lang} = 0;
  $noamp{$lang} = 0;

  while (<STRINGS>) {
    next unless /^(\w*)=\"(.*)\"/;
    my ($dlg,$string) = ($1,$2);
    $string =~ s/&amp;/&/;
    $dlgs{$dlg}{$lang} = $string;

    ## 
    $noamp{$lang}++ unless ($string =~ /\&amp/);
    next unless $string;
    
    ## Set the longest language for a DLG the actual
    ## string for the language can be looked up vi $dlgs
    ## later.
    if ($longest{$dlg}) {
      $longest{$dlg} = $lang
	if length($dlgs{$dlg}{$lang}) > length($dlgs{$dlg}{$longest{$dlg}});
    }
    else {
      $longest{$dlg} = $lang;
    }
  }
}
## Add US into the list
unshift(@lang, 'en-US');

## Determine global missing counts
print STDERR "Missing strings:\n";
$selected_langs=join ":",@ARGV;
foreach my $dlg (keys %dlgs) {
  foreach my $lang (@lang) {
    unless($dlgs{$dlg}{$lang}) {
      print STDERR "$lang: $dlg\n" if $selected_langs =~ $lang;
    #$missing{$lang}++ unless $dlgs{$dlg}{$lang};
      $missing{$lang}++;
    }
  }
}

print
  start_html({bgcolor=>'white'}, ['AbiWord DLG_ Translation Summary']),
  h2('Translation Summaries'),
  "This page summarizes the current state of all known translations in AbiWord.
  Thanks to ", a({href=>"mailto:owen\@pdaverticals.com"},"Owen Stenseth"), 
  " for providing the scripts required to provide this data.<P>";

print "This page was last generated at ";
&PrintTime;
print "<P>";

## Overall Percent Complete
print a({name=>'Percent'}, h2('Percent Complete')),"\n";
my $dlg_count = keys %dlgs;
foreach my $lang (@lang) {
  warn("$lang: $missing{$lang}\n");
  my $percent = sprintf("%3d%", 100 - ($missing{$lang} / $dlg_count) * 100);
  push ( @td, td( [ b($lang) , $percent ]),"\n");
}
print
  table({ border => 1, cellspacing => 0 }, Tr( [ th(['Lang', 'Percent Complete']), @td ] )),"\n";

## Summary Table
my @td;
print a({name=>'Summary'}, h2('Summary')),
  "Green cells are languages with a translation.<BR>"
  ,b("Bold"), " cells are the longest of the translations.<BR>
  White language names are translations with a hot key assigned.<P>";

foreach my $dlg (sort keys %dlgs) {
  next unless $dlg;
  push(@td, td([ a({href => "#$dlg"},b($dlg))]));
  foreach $lang (@lang) {
    my $td;
    if ($dlgs{$dlg}{$lang}) {
      my $longest = $dlgs{$dlg}{$lang} eq $dlgs{$dlg}{$longest{$dlg}};
      my $amp = $dlgs{$dlg}{$lang} =~ /&/;
      $td[$#td] .= td({bgcolor=> '#00AA00'},
		      [ font({color=> $amp ? 'white' : 'black' }, 
			     $longest ? b($lang) : $lang) ]) . "\n";
    }
    else {
      $td[$#td] .= td({bgcolor=>'red'},[ $lang ]);      
    }
  }
}
print
  table({ border => 1, cellspacing => 0 }, 
	Tr( 
	   [ th(['Dialog', @lang]), @td ] 
	  )
       ), "\n";

print
  a({name=>'LongestSummary'}, h2('Longest Language Summary : Sorted by Language'));

my @td = ();
foreach my $dlg (sort { $longest{$a} cmp $longest{$b}  } keys %dlgs) {
  next unless $dlg;
  eval {
    $percent_longer = length($dlgs{$dlg}{'en-US'}) / length($dlgs{$dlg}{$longest{$dlg}}) * 100;
  };
  $percent_longer = sprintf("%3d%", 100 - $percent_longer);
  push(@td, td([ a( { href => "#$dlg"}, b($dlg)), $longest{$dlg}, $longest{$dlg} eq 'en-US' ? 'N/A' : $percent_longer, $dlgs{$dlg}{$longest{$dlg}} ])),"\n";
}  
print
  table({ border => 1, cellspacing => 0 }, 
	Tr( 
	   [ th({valign=>"bottom"},['Dialog', 'Language', ' % Longer<br>than en-US', 'String']), @td ] 
	  )
       ), "\n";

print
  h2('Dialog Details'),"\n";

foreach my $dlg (keys %dlgs) {
  next unless $dlg;
  my @td;
  foreach my $lang (@lang) {
    if (not $dlgs{$dlg}{$lang}) {
      $dlgs{$dlg}{$lang} = "(missing)";
    }
    push(@td, td( [ b($lang), kbd($dlgs{$dlg}{$lang}) ]),"\n")
  }

  print
    a({href=>"#PercentSummary"}, "Percent"), " ",
    a({href=>"#Summary"}, "Summary"), " ",
    a({href=>"#LongestSummary"}, "Longest"),
    p(table({ border => 1, cellspacing => 0 }, 
	  caption(a({name => $dlg}, b($dlg))),
	  Tr( 
	     [ th(['Language', 'String']), @td ] 
	    )
	 )), "\n";
	    
}
