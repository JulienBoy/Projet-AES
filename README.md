# Projet-AES

Dans le cadre de la première année du Master Mathématiques de l'information, cryptographie, nous avions comme projet de faire une implémentation de l'AES en langage C. Il nous était demandé au minimum que notre programme puisse chiffrer et déchiffrer un texte contenu dans un fichier en utilisant le standard AES-128 avec le mode de chiffrement ECB. Pour ma part, j'ai donc inclus ces fonctionnalités et quelques unes de plus dans mon implémentation, notamment le fait de pouvoir chiffrer/déchiffrer en mode CBC, et d'utiliser des clé de 192 et 256 bits. Je vais donc donner une notice d'utilisation de cette implémentation et quelques points d'améliorations possible.

---

## Notice d'utilisation
Dans cette section, est fourni une liste exhaustive des options disponibles ainsi que leurs utilitées. Dans le terminal, une fois dans le bon répertoire faites "make" pour compiler le programme (le compilateur utilisé est gcc) et générer les fichiers binaire, une fois cette étape passée faites "./main" suivi des options choisis pour exécuter le programme.
- '-h' ou '- -help' : cette option permet d'afficher l'aide dans le terminal et d'avoir une courte description des options possible ainsi que leurs usages.
  
- '-c FILE' ou '- -chiffre FILE' : cette option permet de chiffrer le fichier FILE, par défaut FILE sera chiffré en AES mode ECB avec la clé 128 bits par défaut : "000102030405060708090a0b0c0d0e0f". Le chiffré est écrit par défaut dans un fichier "cipher.txt".
- 'd FILE' ou '- -dechiffre FILE' :  cette option permet de déchiffrer le fichier FILE, par défaut FILE sera déchiffré en AES mode ECB avec la clé 128 bits par défaut : "000102030405060708090a0b0c0d0e0f". Le clair est écrit par défaut dans un fichier "plain.txt".
- '-o FILE' ou '- -output FILE' : cette option permet  de choisir le fichier de sortie, c'est-à-dire que si par exemple on chiffre un fichier le chiffré associé sera écrit dans le fichier FILE. Si le FILE existe déjà il sera écrasé.
- '-m MODE' ou '- -mode MODE' : cette option permet de choisir le mode de chiffrement, c'est à dire ECB ou CBC. Le mode par défaut est ECB et que vous écriviez en majuscule ou minuscule le nom du mode cela n'a pas d'importance.
- '-k KEY' ou '- -key KEY' : cette option permet de choisir la clé de chiffrement, toute clé doit être écrit en hexadécimal et doit faire une taille conforme c'est-à-dire 128, 192 ou 256 bits (32, 48 ou 64 hexa). La clé utilisé par défaut est "000102030405060708090a0b0c0d0e0f".
- '-v IV' ou '- -vecteur IV' : cette option permet de choisir le vecteur d'initialisation utilisé pour le mode CBC, tout vecteur doit être écrit en hexadécimal et doit faire une taille conforme c'est-à-dire 128 bits (32 hexa). La vecteur utilisé par défaut est "00112233445566778899aabbccddeeff".
- '-t' ou '- -test' : cette option permet d'effectuer un test de performance. Ce test consiste en chiffrer 100 fois le fichier alice.txt en mode ECB. Dans le terminal sera afficher le temps de calcul pour cette tache mais ce n'est pas le temps réel de l'exécution de la commande entière.
- '-x in/out/all' ou '- -hexa in/out/all' : cette option permet de lire et/ou d'écrire les fichiers en hexadécimal, avec "in" seul un fichier d'entré en hexa peut être lu, avec "out" on va écrire le fichier de sortie en hexa et avec "all" on va faire les deux.

---

Certaines options sont incompatibles. Voici une liste de celles-ci :
- '-c ... -d ...' : incompatible, on ne peut pas chiffrer et déchiffrer en même temps.
- '-t -c ...' : incompatible, si on effectue le test on ne permet pas de chiffrer ou déchiffrer, ni de changer le mode de chiffrement.
- Pour le reste, certaines options associées sont inutiles comme par exemple '-c ... -m ecb -v ...', pour ces cas là aucun message d'erreur n'est envoyé mais un message informatif pour prévenir du fait que cette association n'est pas utile.

---

## Points d'améliorations

L'implémentation proposée ici est, comme mentionné au début, plus complète que le minimum requis. Cependant, elle reste relativement incomplète, notamment en ce qui concerne les modes de chiffrement. Par exemple, les modes OFB (ou CFB) ainsi que le mode GCM, particulièrement pertinent pour garantir l'intégrité des messages, n'ont pas été implémentés.

En termes de performances, mon programme n’est pas particulièrement lent, mais il y a certainement des aspects qui pourraient être optimisés, notamment la gestion de la mémoire. Certaines variables sont initialisées ou modifiées inutilement, ce qui pourrait être amélioré.

Enfin, concernant la clarté du code, bien qu’elle puisse être encore perfectionnée, je pense que le programme reste globalement lisible et compréhensible.
