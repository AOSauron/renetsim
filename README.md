#RENETSIM

Repository Network Simulation

Projet de C TELECOM Nancy 2015-2016

Auteurs
    
    DAUTRICOURT Olivier
    GARCIA Guillaume

Dependances

    libjson0 libjson0-dev
    graphviz graphviz-dev
    libcdt5


Build & execute:

    make
    ./rns [opts]

Utilisation

	Produire un graphe

	    ./rns -i<chemin/vers/fichier> -f<json | rns> -o<fichier/de/sortie> -e<format>
	    Si les formats classiques ne sont pas supportés sur le système svg l'est en général mais a un
	    temps d'écriture plus long.

	Générer un graphe aléatoirement

	    ./rns -r -m<digraph | graph> -n<nbr de dépôts>
	    -l<nbr max d'arcs sortant par dépôt> [-c]

	    (l'option -c permet de renforcer la probabilité que le graphe soit connexe mais ne le garantit pas)

	Trouver le plus court chemin entre deux dépôts

	    ./ rns (-i ... | -r ...) -p -sStart -dDestination


	Exemples :

	    ./rns -iinput/input.rns -frns -ooutput.svg -esvg -p -sA -dD
	    ./rns -r -mdigraph -n10 -l4 -otest.png -epng -c -p -s0 -d9
	    
RNS Format


	SET_GRAPH
	<digraph | graph>:<nombre de dépots>
	
	SET_REPOSITORIES
	<Nom dépôt 1>:<nombre d'arcs sortant du dépôt 1>:(<x1>,<y1>)
	<Nom dépôt 2>:<nombre d'arcs sortant du dépôt 2>:(<x2>,<y2>)
	
	SET_EDGES
	<Nom dépôt 1>-><Nom dépôt 2>:<poids du lien les liant>
	<Nom dépôt 2>-><Nom dépôt 1>:<poids du lien les liant>
