#!/usr/bin/env perl
use warnings;

#----------------------------------------------------------------- TEST 1 -------------------------------------------------------------

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
print "\n\nLancement de tri_sequentiel --quiet --time --rusage <vecteur.txt >>retour.txt\n";
print "Test in progress ... Wait please\n";
for (my $i = 0; $i < 20; $i++) {
	$pid = fork();
	die "Could not fork\n" if not defined $pid;

	if ($pid == 0) {
		exec("./creer_vecteur --size 100000 >vecteur.txt");
	} else {
		waitpid ($pid, 0);
	}

	$pid = fork();
	die "Could not fork\n" if not defined $pid;
	if ($pid == 0) {
		exec("./tri_sequentiel --quiet --time --rusage <vecteur.txt >>retour.txt");
	} else {
		waitpid ($pid, 0);
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

print "Resultat Test 1 : Squentiel avec taille = 100k\n";
my $var;
my $moyenne = 0;
for ($var = 0; $var < scalar(@temps_global); $var++) {
	$moyenne += $temps_global[$var];
}
$moyenne = $moyenne / $var;
print "Temp moyen gloabl en mode sequentielle avec 100k de données : [$moyenne]\n";

$moyenne = 0;
for ($var = 0; $var < scalar(@temps_CPU); $var++) {
	$moyenne += $temps_CPU[$var];
}
$moyenne = $moyenne / $var;
print "Temp moyen CPU en mode sequentielle avec 100k de données : [$moyenne]\n";

$moyenne = 0;
for ($var = 0; $var < scalar(@temps_CPU); $var++) {
	$moyenne += $temps_user[$var];
}
$moyenne = $moyenne / $var;
print "Temp moyen CPU user en mode sequentielle avec 100k de données : [$moyenne]\n";

$moyenne = 0;
for ($var = 0; $var < scalar(@temps_CPU); $var++) {
	$moyenne += $temps_system[$var];
}
$moyenne = $moyenne / $var;
print "Temp moyen CPU system en mode sequentielle avec 100k de données : [$moyenne]\n";
#------------------------------------------------------------- FIN TEST 1 -------------------------------------------------------------

#----------------------------------------------------------------- TEST 2 -------------------------------------------------------------
#Declaration du tableau de valeurs.---------
@temps_global = ();
@temps_CPU = ();
@temps_user = ();
@temps_system = ();

#Vidage du fichier de retour ...
open($fd,">retour.txt") or die("open: $!");
close($fd);
#-------------------------------------------

#Lancement des tests.----------------------------------
print "\n\nLancement du test2 : tri_threads --parallelism 2 --quiet --time --rusage <vecteur.txt >>retour.txt\n";
print "Test in progress ... Wait please\n";
for (my $i = 0; $i < 20; $i++) {
	$pid = fork();
	die "Could not fork\n" if not defined $pid;

	if ($pid == 0) {
		exec("./creer_vecteur --size 100000 >vecteur.txt");
	} else {
		waitpid ($pid, 0);
	}

	$pid = fork();
	die "Could not fork\n" if not defined $pid;
	if ($pid == 0) {
		exec("./tri_threads --parallelism 2 --quiet --time --rusage <vecteur.txt >>retour.txt");
	} else {
		waitpid ($pid, 0);
	}
}
#--------------------------------------------------------------------------------

#lecture du fichier pour recuperer les valeurs et en faire une moyenne.
$compteur = 1;
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

print "Resultat Test 2 : 2 threads avec taille = 100k\n";
$moyenne = 0;
for ($var = 0; $var < scalar(@temps_global); $var++) {
	$moyenne += $temps_global[$var];
}
$moyenne = $moyenne / $var;
print "Temp moyen gloabl en mode sequentielle avec 100k de données : [$moyenne]\n";

$moyenne = 0;
for ($var = 0; $var < scalar(@temps_CPU); $var++) {
	$moyenne += $temps_CPU[$var];
}
$moyenne = $moyenne / $var;
print "Temp moyen CPU en mode sequentielle avec 100k de données : [$moyenne]\n";

$moyenne = 0;
for ($var = 0; $var < scalar(@temps_CPU); $var++) {
	$moyenne += $temps_user[$var];
}
$moyenne = $moyenne / $var;
print "Temp moyen CPU user en mode sequentielle avec 100k de données : [$moyenne]\n";

$moyenne = 0;
for ($var = 0; $var < scalar(@temps_CPU); $var++) {
	$moyenne += $temps_system[$var];
}
$moyenne = $moyenne / $var;
print "Temp moyen CPU system en mode sequentielle avec 100k de données : [$moyenne]\n";
#------------------------------------------------------------- FIN TEST 2 -------------------------------------------------------------

#----------------------------------------------------------------- TEST 3 -------------------------------------------------------------
#Declaration du tableau de valeurs.---------
@temps_global = ();
@temps_CPU = ();
@temps_user = ();
@temps_system = ();

#Vidage du fichier de retour ...
open($fd,">retour.txt") or die("open: $!");
close($fd);
#-------------------------------------------

#Lancement des tests en mode sequentiel.----------------------------------
print "\n\nLancement du test3 : tri_threads --parallelism 4 --quiet --time --rusage <vecteur.txt >>retour.txt\n";
print "Test in progress ... Wait please\n";
for (my $i = 0; $i < 20; $i++) {
	$pid = fork();
	die "Could not fork\n" if not defined $pid;

	if ($pid == 0) {
		exec("./creer_vecteur --size 100000 >vecteur.txt");
	} else {
		waitpid ($pid, 0);
	}

	$pid = fork();
	die "Could not fork\n" if not defined $pid;
	if ($pid == 0) {
		exec("./tri_threads --parallelism 4 --quiet --time --rusage <vecteur.txt >>retour.txt");
	} else {
		waitpid ($pid, 0);
	}
}
#--------------------------------------------------------------------------------

#lecture du fichier pour recuperer les valeurs et en faire une moyenne.
$compteur = 1;
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

print "Resultat Test 3 : 4 threads avec taille = 100k\n";
$moyenne = 0;
for ($var = 0; $var < scalar(@temps_global); $var++) {
	$moyenne += $temps_global[$var];
}
$moyenne = $moyenne / $var;
print "Temp moyen gloabl en mode sequentielle avec 100k de données : [$moyenne]\n";

$moyenne = 0;
for ($var = 0; $var < scalar(@temps_CPU); $var++) {
	$moyenne += $temps_CPU[$var];
}
$moyenne = $moyenne / $var;
print "Temp moyen CPU en mode sequentielle avec 100k de données : [$moyenne]\n";

$moyenne = 0;
for ($var = 0; $var < scalar(@temps_CPU); $var++) {
	$moyenne += $temps_user[$var];
}
$moyenne = $moyenne / $var;
print "Temp moyen CPU user en mode sequentielle avec 100k de données : [$moyenne]\n";

$moyenne = 0;
for ($var = 0; $var < scalar(@temps_CPU); $var++) {
	$moyenne += $temps_system[$var];
}
$moyenne = $moyenne / $var;
print "Temp moyen CPU system en mode sequentielle avec 100k de données : [$moyenne]\n";
#------------------------------------------------------------- FIN TEST 3 -------------------------------------------------------------

#----------------------------------------------------------------- TEST 4 -------------------------------------------------------------
#Declaration du tableau de valeurs.---------
@temps_global = ();
@temps_CPU = ();
@temps_user = ();
@temps_system = ();

#Vidage du fichier de retour ...
open($fd,">retour.txt") or die("open: $!");
close($fd);
#-------------------------------------------

#Lancement des tests en mode sequentiel.----------------------------------
print "\n\nLancement du test4 : tri_threads --parallelism 8 --quiet --time --rusage <vecteur.txt >>retour.txt\n";
print "Test in progress ... Wait please\n";
for (my $i = 0; $i < 20; $i++) {
	$pid = fork();
	die "Could not fork\n" if not defined $pid;

	if ($pid == 0) {
		exec("./creer_vecteur --size 100000 >vecteur.txt");
	} else {
		waitpid ($pid, 0);
	}

	$pid = fork();
	die "Could not fork\n" if not defined $pid;
	if ($pid == 0) {
		exec("./tri_threads --parallelism 8 --quiet --time --rusage <vecteur.txt >>retour.txt");
	} else {
		waitpid ($pid, 0);
	}
}
#--------------------------------------------------------------------------------

#lecture du fichier pour recuperer les valeurs et en faire une moyenne.
$compteur = 1;
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

print "Resultat Test 4 : 8 threads avec taille = 100k\n";
$moyenne = 0;
for ($var = 0; $var < scalar(@temps_global); $var++) {
	$moyenne += $temps_global[$var];
}
$moyenne = $moyenne / $var;
print "Temp moyen gloabl en mode sequentielle avec 100k de données : [$moyenne]\n";

$moyenne = 0;
for ($var = 0; $var < scalar(@temps_CPU); $var++) {
	$moyenne += $temps_CPU[$var];
}
$moyenne = $moyenne / $var;
print "Temp moyen CPU en mode sequentielle avec 100k de données : [$moyenne]\n";

$moyenne = 0;
for ($var = 0; $var < scalar(@temps_CPU); $var++) {
	$moyenne += $temps_user[$var];
}
$moyenne = $moyenne / $var;
print "Temp moyen CPU user en mode sequentielle avec 100k de données : [$moyenne]\n";

$moyenne = 0;
for ($var = 0; $var < scalar(@temps_CPU); $var++) {
	$moyenne += $temps_system[$var];
}
$moyenne = $moyenne / $var;
print "Temp moyen CPU system en mode sequentielle avec 100k de données : [$moyenne]\n";
#------------------------------------------------------------- FIN TEST 4 -------------------------------------------------------------

#----------------------------------------------------------------- TEST 5 -------------------------------------------------------------

#Declaration du tableau de valeurs.---------
@temps_global = ();
@temps_CPU = ();
@temps_user = ();
@temps_system = ();
#Vidage du fichier de retour ...
open($fd,">retour.txt") or die("open: $!");
close($fd);
#-------------------------------------------

#Lancement des tests en mode sequentiel.----------------------------------
print "\n\nLancement de tri_sequentiel --quiet --time --rusage <vecteur.txt >>retour.txt\n";
print "Test in progress ... Wait please\n";
for (my $i = 0; $i < 20; $i++) {
	$pid = fork();
	die "Could not fork\n" if not defined $pid;

	if ($pid == 0) {
		exec("./creer_vecteur --size 400000 >vecteur.txt");
	} else {
		waitpid ($pid, 0);
	}

	$pid = fork();
	die "Could not fork\n" if not defined $pid;
	if ($pid == 0) {
		exec("./tri_sequentiel --quiet --time --rusage <vecteur.txt >>retour.txt");
	} else {
		waitpid ($pid, 0);
	}
}
#--------------------------------------------------------------------------------

#lecture du fichier pour recuperer les valeurs et en faire une moyenne.
	#ligne 1 :  temps global 	"113058"
	#ligne 2 : 	temps cpu		"112972 111483 1489"	<temps CPU> <temps user> <temps system>
$compteur = 1;
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

print "Resultat Test 5 : Squentiel avec taille = 400k\n";
$moyenne = 0;
for ($var = 0; $var < scalar(@temps_global); $var++) {
	$moyenne += $temps_global[$var];
}
$moyenne = $moyenne / $var;
print "Temp moyen gloabl en mode sequentielle avec 400k de données : [$moyenne]\n";

$moyenne = 0;
for ($var = 0; $var < scalar(@temps_CPU); $var++) {
	$moyenne += $temps_CPU[$var];
}
$moyenne = $moyenne / $var;
print "Temp moyen CPU en mode sequentielle avec 400k de données : [$moyenne]\n";

$moyenne = 0;
for ($var = 0; $var < scalar(@temps_CPU); $var++) {
	$moyenne += $temps_user[$var];
}
$moyenne = $moyenne / $var;
print "Temp moyen CPU user en mode sequentielle avec 400k de données : [$moyenne]\n";

$moyenne = 0;
for ($var = 0; $var < scalar(@temps_CPU); $var++) {
	$moyenne += $temps_system[$var];
}
$moyenne = $moyenne / $var;
print "Temp moyen CPU system en mode sequentielle avec 400k de données : [$moyenne]\n";
#------------------------------------------------------------- FIN TEST 5 -------------------------------------------------------------

#----------------------------------------------------------------- TEST 6 -------------------------------------------------------------
#Declaration du tableau de valeurs.---------
@temps_global = ();
@temps_CPU = ();
@temps_user = ();
@temps_system = ();

#Vidage du fichier de retour ...
open($fd,">retour.txt") or die("open: $!");
close($fd);
#-------------------------------------------

#Lancement des tests.----------------------------------
print "\n\nLancement du test6 : tri_threads --parallelism 2 --quiet --time --rusage <vecteur.txt >>retour.txt\n";
print "Test in progress ... Wait please\n";
for (my $i = 0; $i < 20; $i++) {
	$pid = fork();
	die "Could not fork\n" if not defined $pid;

	if ($pid == 0) {
		exec("./creer_vecteur --size 400000 >vecteur.txt");
	} else {
		waitpid ($pid, 0);
	}

	$pid = fork();
	die "Could not fork\n" if not defined $pid;
	if ($pid == 0) {
		exec("./tri_threads --parallelism 2 --quiet --time --rusage <vecteur.txt >>retour.txt");
	} else {
		waitpid ($pid, 0);
	}
}
#--------------------------------------------------------------------------------

#lecture du fichier pour recuperer les valeurs et en faire une moyenne.
$compteur = 1;
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

print "Resultat Test 6 : 2 threads avec taille = 400k\n";
$moyenne = 0;
for ($var = 0; $var < scalar(@temps_global); $var++) {
	$moyenne += $temps_global[$var];
}
$moyenne = $moyenne / $var;
print "Temp moyen gloabl en mode sequentielle avec 400k de données : [$moyenne]\n";

$moyenne = 0;
for ($var = 0; $var < scalar(@temps_CPU); $var++) {
	$moyenne += $temps_CPU[$var];
}
$moyenne = $moyenne / $var;
print "Temp moyen CPU en mode sequentielle avec 400k de données : [$moyenne]\n";

$moyenne = 0;
for ($var = 0; $var < scalar(@temps_CPU); $var++) {
	$moyenne += $temps_user[$var];
}
$moyenne = $moyenne / $var;
print "Temp moyen CPU user en mode sequentielle avec 400k de données : [$moyenne]\n";

$moyenne = 0;
for ($var = 0; $var < scalar(@temps_CPU); $var++) {
	$moyenne += $temps_system[$var];
}
$moyenne = $moyenne / $var;
print "Temp moyen CPU system en mode sequentielle avec 400k de données : [$moyenne]\n";
#------------------------------------------------------------- FIN TEST 6 -------------------------------------------------------------

#----------------------------------------------------------------- TEST 7 -------------------------------------------------------------
#Declaration du tableau de valeurs.---------
@temps_global = ();
@temps_CPU = ();
@temps_user = ();
@temps_system = ();

#Vidage du fichier de retour ...
open($fd,">retour.txt") or die("open: $!");
close($fd);
#-------------------------------------------

#Lancement des tests en mode sequentiel.----------------------------------
print "\n\nLancement du test7 : tri_threads --parallelism 4 --quiet --time --rusage <vecteur.txt >>retour.txt\n";
print "Test in progress ... Wait please\n";
for (my $i = 0; $i < 20; $i++) {
	$pid = fork();
	die "Could not fork\n" if not defined $pid;

	if ($pid == 0) {
		exec("./creer_vecteur --size 400000 >vecteur.txt");
	} else {
		waitpid ($pid, 0);
	}

	$pid = fork();
	die "Could not fork\n" if not defined $pid;
	if ($pid == 0) {
		exec("./tri_threads --parallelism 4 --quiet --time --rusage <vecteur.txt >>retour.txt");
	} else {
		waitpid ($pid, 0);
	}
}
#--------------------------------------------------------------------------------

#lecture du fichier pour recuperer les valeurs et en faire une moyenne.
$compteur = 1;
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

print "Resultat Test 7 : 4 threads avec taille = 400k\n";
$moyenne = 0;
for ($var = 0; $var < scalar(@temps_global); $var++) {
	$moyenne += $temps_global[$var];
}
$moyenne = $moyenne / $var;
print "Temp moyen gloabl en mode sequentielle avec 400k de données : [$moyenne]\n";

$moyenne = 0;
for ($var = 0; $var < scalar(@temps_CPU); $var++) {
	$moyenne += $temps_CPU[$var];
}
$moyenne = $moyenne / $var;
print "Temp moyen CPU en mode sequentielle avec 400k de données : [$moyenne]\n";

$moyenne = 0;
for ($var = 0; $var < scalar(@temps_CPU); $var++) {
	$moyenne += $temps_user[$var];
}
$moyenne = $moyenne / $var;
print "Temp moyen CPU user en mode sequentielle avec 400k de données : [$moyenne]\n";

$moyenne = 0;
for ($var = 0; $var < scalar(@temps_CPU); $var++) {
	$moyenne += $temps_system[$var];
}
$moyenne = $moyenne / $var;
print "Temp moyen CPU system en mode sequentielle avec 400k de données : [$moyenne]\n";
#------------------------------------------------------------- FIN TEST 7 -------------------------------------------------------------

#----------------------------------------------------------------- TEST 8 -------------------------------------------------------------
#Declaration du tableau de valeurs.---------
@temps_global = ();
@temps_CPU = ();
@temps_user = ();
@temps_system = ();

#Vidage du fichier de retour ...
open($fd,">retour.txt") or die("open: $!");
close($fd);
#-------------------------------------------

#Lancement des tests en mode sequentiel.----------------------------------
print "\n\nLancement du test8 : tri_threads --parallelism 8 --quiet --time --rusage <vecteur.txt >>retour.txt\n";
print "Test in progress ... Wait please\n";
for (my $i = 0; $i < 20; $i++) {
	$pid = fork();
	die "Could not fork\n" if not defined $pid;

	if ($pid == 0) {
		exec("./creer_vecteur --size 400000 >vecteur.txt");
	} else {
		waitpid ($pid, 0);
	}

	$pid = fork();
	die "Could not fork\n" if not defined $pid;
	if ($pid == 0) {
		exec("./tri_threads --parallelism 8 --quiet --time --rusage <vecteur.txt >>retour.txt");
	} else {
		waitpid ($pid, 0);
	}
}
#--------------------------------------------------------------------------------

#lecture du fichier pour recuperer les valeurs et en faire une moyenne.
$compteur = 1;
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

print "Resultat Test 8 : 8 threads avec taille = 400k\n";
$moyenne = 0;
for ($var = 0; $var < scalar(@temps_global); $var++) {
	$moyenne += $temps_global[$var];
}
$moyenne = $moyenne / $var;
print "Temp moyen gloabl en mode sequentielle avec 400k de données : [$moyenne]\n";

$moyenne = 0;
for ($var = 0; $var < scalar(@temps_CPU); $var++) {
	$moyenne += $temps_CPU[$var];
}
$moyenne = $moyenne / $var;
print "Temp moyen CPU en mode sequentielle avec 400k de données : [$moyenne]\n";

$moyenne = 0;
for ($var = 0; $var < scalar(@temps_CPU); $var++) {
	$moyenne += $temps_user[$var];
}
$moyenne = $moyenne / $var;
print "Temp moyen CPU user en mode sequentielle avec 400k de données : [$moyenne]\n";

$moyenne = 0;
for ($var = 0; $var < scalar(@temps_CPU); $var++) {
	$moyenne += $temps_system[$var];
}
$moyenne = $moyenne / $var;
print "Temp moyen CPU system en mode sequentielle avec 400k de données : [$moyenne]\n";
#------------------------------------------------------------- FIN TEST 8 -------------------------------------------------------------

#----------------------------------------------------------------- TEST 9 -------------------------------------------------------------
#Declaration du tableau de valeurs.---------
@temps_global = ();
@temps_CPU = ();
@temps_user = ();
@temps_system = ();

#Vidage du fichier de retour ...
open($fd,">retour.txt") or die("open: $!");
close($fd);
#-------------------------------------------

#Lancement des tests en mode sequentiel.----------------------------------
print "\n\nLancement du test9 : tri_sequentiel --quiet --time --rusage <vecteur.txt >>retour.txt\n";
print "Test in progress ... Wait please\n";
for (my $i = 0; $i < 20; $i++) {
	$pid = fork();
	die "Could not fork\n" if not defined $pid;

	if ($pid == 0) {
		exec("./creer_vecteur --size 700000 >vecteur.txt");
	} else {
		waitpid ($pid, 0);
	}

	$pid = fork();
	die "Could not fork\n" if not defined $pid;
	if ($pid == 0) {
		exec("./tri_sequentiel --quiet --time --rusage <vecteur.txt >>retour.txt");
	} else {
		waitpid ($pid, 0);
	}
}
#--------------------------------------------------------------------------------

#lecture du fichier pour recuperer les valeurs et en faire une moyenne.
$compteur = 1;
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

print "Resultat Test 9 : Sequentiel avec taille = 700k\n";
$moyenne = 0;
for ($var = 0; $var < scalar(@temps_global); $var++) {
	$moyenne += $temps_global[$var];
}
$moyenne = $moyenne / $var;
print "Temp moyen gloabl en mode sequentielle avec 700k de données : [$moyenne]\n";

$moyenne = 0;
for ($var = 0; $var < scalar(@temps_CPU); $var++) {
	$moyenne += $temps_CPU[$var];
}
$moyenne = $moyenne / $var;
print "Temp moyen CPU en mode sequentielle avec 700k de données : [$moyenne]\n";

$moyenne = 0;
for ($var = 0; $var < scalar(@temps_CPU); $var++) {
	$moyenne += $temps_user[$var];
}
$moyenne = $moyenne / $var;
print "Temp moyen CPU user en mode sequentielle avec 700k de données : [$moyenne]\n";

$moyenne = 0;
for ($var = 0; $var < scalar(@temps_CPU); $var++) {
	$moyenne += $temps_system[$var];
}
$moyenne = $moyenne / $var;
print "Temp moyen CPU system en mode sequentielle avec 700k de données : [$moyenne]\n";
#------------------------------------------------------------- FIN TEST 9 -------------------------------------------------------------

#----------------------------------------------------------------- TEST 10 -------------------------------------------------------------
#Declaration du tableau de valeurs.---------
@temps_global = ();
@temps_CPU = ();
@temps_user = ();
@temps_system = ();

#Vidage du fichier de retour ...
open($fd,">retour.txt") or die("open: $!");
close($fd);
#-------------------------------------------

#Lancement des tests.----------------------------------
print "\n\nLancement du test10 : tri_threads --parallelism 2 --quiet --time --rusage <vecteur.txt >>retour.txt\n";
print "Test in progress ... Wait please\n";
for (my $i = 0; $i < 20; $i++) {
	$pid = fork();
	die "Could not fork\n" if not defined $pid;

	if ($pid == 0) {
		exec("./creer_vecteur --size 700000 >vecteur.txt");
	} else {
		waitpid ($pid, 0);
	}

	$pid = fork();
	die "Could not fork\n" if not defined $pid;
	if ($pid == 0) {
		exec("./tri_threads --parallelism 2 --quiet --time --rusage <vecteur.txt >>retour.txt");
	} else {
		waitpid ($pid, 0);
	}
}
#--------------------------------------------------------------------------------

#lecture du fichier pour recuperer les valeurs et en faire une moyenne.
$compteur = 1;
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

print "Resultat Test 10 : 2 threads avec taille = 700k\n";
$moyenne = 0;
for ($var = 0; $var < scalar(@temps_global); $var++) {
	$moyenne += $temps_global[$var];
}
$moyenne = $moyenne / $var;
print "Temp moyen gloabl en mode sequentielle avec 700k de données : [$moyenne]\n";

$moyenne = 0;
for ($var = 0; $var < scalar(@temps_CPU); $var++) {
	$moyenne += $temps_CPU[$var];
}
$moyenne = $moyenne / $var;
print "Temp moyen CPU en mode sequentielle avec 700k de données : [$moyenne]\n";

$moyenne = 0;
for ($var = 0; $var < scalar(@temps_CPU); $var++) {
	$moyenne += $temps_user[$var];
}
$moyenne = $moyenne / $var;
print "Temp moyen CPU user en mode sequentielle avec 700k de données : [$moyenne]\n";

$moyenne = 0;
for ($var = 0; $var < scalar(@temps_CPU); $var++) {
	$moyenne += $temps_system[$var];
}
$moyenne = $moyenne / $var;
print "Temp moyen CPU system en mode sequentielle avec 700k de données : [$moyenne]\n";
#------------------------------------------------------------- FIN TEST 10 -------------------------------------------------------------

#----------------------------------------------------------------- TEST 11 -------------------------------------------------------------
#Declaration du tableau de valeurs.---------
@temps_global = ();
@temps_CPU = ();
@temps_user = ();
@temps_system = ();

#Vidage du fichier de retour ...
open($fd,">retour.txt") or die("open: $!");
close($fd);
#-------------------------------------------

#Lancement des tests en mode sequentiel.----------------------------------
print "\n\nLancement du test11 : tri_threads --parallelism 4 --quiet --time --rusage <vecteur.txt >>retour.txt\n";
print "Test in progress ... Wait please\n";
for (my $i = 0; $i < 20; $i++) {
	$pid = fork();
	die "Could not fork\n" if not defined $pid;

	if ($pid == 0) {
		exec("./creer_vecteur --size 700000 >vecteur.txt");
	} else {
		waitpid ($pid, 0);
	}

	$pid = fork();
	die "Could not fork\n" if not defined $pid;
	if ($pid == 0) {
		exec("./tri_threads --parallelism 4 --quiet --time --rusage <vecteur.txt >>retour.txt");
	} else {
		waitpid ($pid, 0);
	}
}
#--------------------------------------------------------------------------------

#lecture du fichier pour recuperer les valeurs et en faire une moyenne.
$compteur = 1;
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

print "Resultat Test 11 : 4 threads avec taille = 700k\n";
$moyenne = 0;
for ($var = 0; $var < scalar(@temps_global); $var++) {
	$moyenne += $temps_global[$var];
}
$moyenne = $moyenne / $var;
print "Temp moyen gloabl en mode sequentielle avec 700k de données : [$moyenne]\n";

$moyenne = 0;
for ($var = 0; $var < scalar(@temps_CPU); $var++) {
	$moyenne += $temps_CPU[$var];
}
$moyenne = $moyenne / $var;
print "Temp moyen CPU en mode sequentielle avec 700k de données : [$moyenne]\n";

$moyenne = 0;
for ($var = 0; $var < scalar(@temps_CPU); $var++) {
	$moyenne += $temps_user[$var];
}
$moyenne = $moyenne / $var;
print "Temp moyen CPU user en mode sequentielle avec 700k de données : [$moyenne]\n";

$moyenne = 0;
for ($var = 0; $var < scalar(@temps_CPU); $var++) {
	$moyenne += $temps_system[$var];
}
$moyenne = $moyenne / $var;
print "Temp moyen CPU system en mode sequentielle avec 700k de données : [$moyenne]\n";
#------------------------------------------------------------- FIN TEST 11 -------------------------------------------------------------

#----------------------------------------------------------------- TEST 12 -------------------------------------------------------------
#Declaration du tableau de valeurs.---------
@temps_global = ();
@temps_CPU = ();
@temps_user = ();
@temps_system = ();

#Vidage du fichier de retour ...
open($fd,">retour.txt") or die("open: $!");
close($fd);
#-------------------------------------------

#Lancement des tests en mode sequentiel.----------------------------------
print "\n\nLancement du test12 : tri_threads --parallelism 8 --quiet --time --rusage <vecteur.txt >>retour.txt\n";
print "Test in progress ... Wait please\n";
for (my $i = 0; $i < 20; $i++) {
	$pid = fork();
	die "Could not fork\n" if not defined $pid;

	if ($pid == 0) {
		exec("./creer_vecteur --size 700000 >vecteur.txt");
	} else {
		waitpid ($pid, 0);
	}

	$pid = fork();
	die "Could not fork\n" if not defined $pid;
	if ($pid == 0) {
		exec("./tri_threads --parallelism 8 --quiet --time --rusage <vecteur.txt >>retour.txt");
	} else {
		waitpid ($pid, 0);
	}
}
#--------------------------------------------------------------------------------

#lecture du fichier pour recuperer les valeurs et en faire une moyenne.
$compteur = 1;
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

print "Resultat Test 12 : 8 threads avec taille = 700k\n";
$moyenne = 0;
for ($var = 0; $var < scalar(@temps_global); $var++) {
	$moyenne += $temps_global[$var];
}
$moyenne = $moyenne / $var;
print "Temp moyen gloabl en mode sequentielle avec 700k de données : [$moyenne]\n";

$moyenne = 0;
for ($var = 0; $var < scalar(@temps_CPU); $var++) {
	$moyenne += $temps_CPU[$var];
}
$moyenne = $moyenne / $var;
print "Temp moyen CPU en mode sequentielle avec 700k de données : [$moyenne]\n";

$moyenne = 0;
for ($var = 0; $var < scalar(@temps_CPU); $var++) {
	$moyenne += $temps_user[$var];
}
$moyenne = $moyenne / $var;
print "Temp moyen CPU user en mode sequentielle avec 700k de données : [$moyenne]\n";

$moyenne = 0;
for ($var = 0; $var < scalar(@temps_CPU); $var++) {
	$moyenne += $temps_system[$var];
}
$moyenne = $moyenne / $var;
print "Temp moyen CPU system en mode sequentielle avec 700k de données : [$moyenne]\n";
#------------------------------------------------------------- FIN TEST 12 -------------------------------------------------------------

#----------------------------------------------------------------- TEST 13 -------------------------------------------------------------

#Declaration du tableau de valeurs.---------
@temps_global = ();
@temps_CPU = ();
@temps_user = ();
@temps_system = ();
#Vidage du fichier de retour ...
open($fd,">retour.txt") or die("open: $!");
close($fd);
#-------------------------------------------

#Lancement des tests en mode sequentiel.----------------------------------
print "\n\nLancement de tri_sequentiel --quiet --time --rusage <vecteur.txt >>retour.txt\n";
print "Test in progress ... Wait please\n";
for (my $i = 0; $i < 20; $i++) {
	$pid = fork();
	die "Could not fork\n" if not defined $pid;

	if ($pid == 0) {
		exec("./creer_vecteur --size 1000000 >vecteur.txt");
	} else {
		waitpid ($pid, 0);
	}

	$pid = fork();
	die "Could not fork\n" if not defined $pid;
	if ($pid == 0) {
		exec("./tri_sequentiel --quiet --time --rusage <vecteur.txt >>retour.txt");
	} else {
		waitpid ($pid, 0);
	}
}
#--------------------------------------------------------------------------------

#lecture du fichier pour recuperer les valeurs et en faire une moyenne.
	#ligne 1 :  temps global 	"113058"
	#ligne 2 : 	temps cpu		"112972 111483 1489"	<temps CPU> <temps user> <temps system>
$compteur = 1;
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

print "Resultat Test 13 : Squentiel avec taille = 1M\n";
$moyenne = 0;
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
#------------------------------------------------------------- FIN TEST 13 -------------------------------------------------------------

#----------------------------------------------------------------- TEST 14 -------------------------------------------------------------
#Declaration du tableau de valeurs.---------
@temps_global = ();
@temps_CPU = ();
@temps_user = ();
@temps_system = ();

#Vidage du fichier de retour ...
open($fd,">retour.txt") or die("open: $!");
close($fd);
#-------------------------------------------

#Lancement des tests en mode sequentiel.----------------------------------
print "\n\nLancement du test14 : tri_threads --parallelism 2 --quiet --time --rusage <vecteur.txt >>retour.txt\n";
print "Test in progress ... Wait please\n";
for (my $i = 0; $i < 20; $i++) {
	$pid = fork();
	die "Could not fork\n" if not defined $pid;

	if ($pid == 0) {
		exec("./creer_vecteur --size 1000000 >vecteur.txt");
	} else {
		waitpid ($pid, 0);
	}

	$pid = fork();
	die "Could not fork\n" if not defined $pid;
	if ($pid == 0) {
		exec("./tri_threads --parallelism 2 --quiet --time --rusage <vecteur.txt >>retour.txt");
	} else {
		waitpid ($pid, 0);
	}
}
#--------------------------------------------------------------------------------

#lecture du fichier pour recuperer les valeurs et en faire une moyenne.
$compteur = 1;
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

print "Resultat Test 14 : 2 threads avec taille = 1M\n";
$moyenne = 0;
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
#------------------------------------------------------------- FIN TEST 14 -------------------------------------------------------------

#----------------------------------------------------------------- TEST 15 -------------------------------------------------------------
#Declaration du tableau de valeurs.---------
@temps_global = ();
@temps_CPU = ();
@temps_user = ();
@temps_system = ();

#Vidage du fichier de retour ...
open($fd,">retour.txt") or die("open: $!");
close($fd);
#-------------------------------------------

#Lancement des tests en mode sequentiel.----------------------------------
print "\n\nLancement du test15 : tri_threads --parallelism 4 --quiet --time --rusage <vecteur.txt >>retour.txt\n";
print "Test in progress ... Wait please\n";
for (my $i = 0; $i < 20; $i++) {
	$pid = fork();
	die "Could not fork\n" if not defined $pid;

	if ($pid == 0) {
		exec("./creer_vecteur --size 1000000 >vecteur.txt");
	} else {
		waitpid ($pid, 0);
	}

	$pid = fork();
	die "Could not fork\n" if not defined $pid;
	if ($pid == 0) {
		exec("./tri_threads --parallelism 4 --quiet --time --rusage <vecteur.txt >>retour.txt");
	} else {
		waitpid ($pid, 0);
	}
}
#--------------------------------------------------------------------------------

#lecture du fichier pour recuperer les valeurs et en faire une moyenne.
$compteur = 1;
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

print "Resultat Test 15 : 4 threads avec taille = 1M\n";
$moyenne = 0;
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
#------------------------------------------------------------- FIN TEST 15 -------------------------------------------------------------

#----------------------------------------------------------------- TEST 16 -------------------------------------------------------------
#Declaration du tableau de valeurs.---------
@temps_global = ();
@temps_CPU = ();
@temps_user = ();
@temps_system = ();

#Vidage du fichier de retour ...
open($fd,">retour.txt") or die("open: $!");
close($fd);
#-------------------------------------------

#Lancement des tests en mode sequentiel.----------------------------------
print "\n\nLancement du test16 : tri_threads --parallelism 8 --quiet --time --rusage <vecteur.txt >>retour.txt\n";
print "Test in progress ... Wait please\n";
for (my $i = 0; $i < 20; $i++) {
	$pid = fork();
	die "Could not fork\n" if not defined $pid;

	if ($pid == 0) {
		exec("./creer_vecteur --size 1000000 >vecteur.txt");
	} else {
		waitpid ($pid, 0);
	}

	$pid = fork();
	die "Could not fork\n" if not defined $pid;
	if ($pid == 0) {
		exec("./tri_threads --parallelism 8 --quiet --time --rusage <vecteur.txt >>retour.txt");
	} else {
		waitpid ($pid, 0);
	}
}
#--------------------------------------------------------------------------------

#lecture du fichier pour recuperer les valeurs et en faire une moyenne.
$compteur = 1;
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

print "Resultat Test 16 : 8 threads avec taille = 1M\n";
$moyenne = 0;
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
#------------------------------------------------------------- FIN TEST 16 -------------------------------------------------------------