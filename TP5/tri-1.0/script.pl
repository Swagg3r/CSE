#!/usr/bin/env perl
use warnings;

#Declaration du tableau de valeurs.---------
my @temps_global = ();
my @temps_CPU = ();
my @temps_user = ();
my @temps_system = ();
my $pid;
#Vidage du fichier de retour ...
my $fd;
open($fd,">retour.txt") or die("open: $!");
close($fd);
#-------------------------------------------

#Lancement des tests en mode sequentiel.----------------------------------
print "Lancement de ./tri_sequentiel\n";
for (my $i = 0; $i < 20; $i++) {
	$pid = fork();
	die "Could not fork\n" if not defined $pid;

	if ($pid == 0) {
		exec("./creer_vecteur --size 1000000 >vecteur.txt");
	} else {
		waitpid ($pid, 0);
	 	print "Vecteur créer.\n";
	}

	$pid = fork();
	die "Could not fork\n" if not defined $pid;
	if ($pid == 0) {
		exec("./tri_sequentiel --quiet --time --rusage <vecteur.txt >>retour.txt");
	} else {
		my $res = waitpid ($pid, 0);
		print "tri_sequentiel [$i] Fin\n";
	}
}
#--------------------------------------------------------------------------------

#lecture du fichier pour recuperer les valeurs et en faire une moyenne.
	#ligne 1 :  temps global 	"113058"
	#ligne 2 : 	temps cpu		"112972 111483 1489"	<temps CPU> <temps user> <temps system>
my $fd_retour;
my $line;
my $compteur = 1;
open($fd_retour,"<retour.txt") or die("open: $!");
while ($line = <$fd_retour>) {
		chomp $line;
	if ($compteur%2 == 1) {
		push(@temps_global,$line);
	} else {
		my @temporaire = split(/ /,$line);
		push(@temps_CPU,$temporaire[0]);
		push(@temps_user,$temporaire[1]);
		push(@temps_system,$temporaire[2]);
	}
	$compteur++;
}
close($fd_retour);
#----------------------------------------------------------------------


my $var;
my $moyenne = 0;
for ($var = 0; $var < scalar(@temps_global); $var++) {
	$moyenne += $temps_global[$var];
}
$moyenne = $moyenne / $var;
print "Temp moyen gloabl en mode sequentielle avec 1M de données : [$moyenne]\n";

$moyenne = 0;
for ($var = 0; $var < scalar(@temps_CPU); $var++) {
	$moyenne += $temps_CPU[$var];
}
$moyenne = $moyenne / $var;
print "Temp moyen CPU en mode sequentielle avec 1M de données : [$moyenne]\n";

$moyenne = 0;
for ($var = 0; $var < scalar(@temps_CPU); $var++) {
	$moyenne += $temps_user[$var];
}
$moyenne = $moyenne / $var;
print "Temp moyen CPU user en mode sequentielle avec 1M de données : [$moyenne]\n";

$moyenne = 0;
for ($var = 0; $var < scalar(@temps_CPU); $var++) {
	$moyenne += $temps_system[$var];
}
$moyenne = $moyenne / $var;
print "Temp moyen CPU system en mode sequentielle avec 1M de données : [$moyenne]\n";