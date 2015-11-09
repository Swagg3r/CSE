#!/usr/bin/env perl
use warnings;

#Vidage du fichier de retour ...
my $fd;
open($fd,">retour.txt") or die("open: $!");
close($fd);

#Creation du vecteur et attente de la terminaison ...
print "Lancement de ./creer_vecteur --size 1000000 >vecteur.txt\n";
my $pid = fork();
die "Could not fork\n" if not defined $pid;

if ($pid == 0) {
	exec("./creer_vecteur --size 1000000 >vecteur.txt");
} else {
	waitpid ($pid, 0);
 	print "Vecteur créer.\n";
}

#Lancement du premier test en mode sequentiel.
print "Lancement de ./tri_sequentiel\n";
$pid = fork();
die "Could not fork\n" if not defined $pid;
if ($pid == 0) {
	exec("./tri_sequentiel --quiet --time --rusage <vecteur.txt >>retour.txt");
} else {
	my $res = waitpid ($pid, 0);
	print "tri_sequentiel Fin\n";
}

#lecture du fichier pour recuperer les valeurs et en faire une moyenne.
	#ligne 1 :  temps global 	"113058"
	#ligne 2 : 	temps cpu		"112972 111483 1489"	<temps CPU> <temps user> <temps system>
