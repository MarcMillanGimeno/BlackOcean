CREADO POR:
	Miguel �ngel D�az Rodr�guez ls31284
	Marc Mill�n Gimeno ls29307

Proyecto: 
	BLACK OCEAN 

YOUTUBE: https://youtu.be/ojbwyqJy7Bg

Descripcion:

	Para la implementaci�n de este videojuego hemos utilizado Visual Studio, un entorno
	de desarrollo integrado (IDE) para sistemas operativos Windows. Programando en C#.

	Para la producci�n de este videojuego primero se program� desde practicamente cero un motor 
	gr�fico que nos permite leer assets, como objetos dise�ados en 3D, en formato .obj, texturas,...
	
	Una vez implemenentada la primera parte que nos permit�a cargar "resources", assets mediante su informaci�n en el .tga
	y sus texturas, se programo el main loop, que ser� el puente a poder acceder al Men� principal, la pantalla de espera
	(Loading...), el nivel 1 y el modo de "Pause". Tambi�n se implement� la clase GameObjectMesh, GameObjectLight, GameObjectEnviroment,
	muy importantes, que contendr� la geometr�a, textura, material... de cada objecto. Desde ese punto de implement� la herencia de objetos,
	y se carg� una esfera con una textura ambientada en el espacio.
	
	Seguidamente, aparecio la clase Scene, que nos permiti� mediante un rootNode, linkear cada uno de los objetos para poder acceder
	comodamente y rapidamente a cada uno de ellos.
	
 	Implementamos tambi�n  para  poder utilizar XML (eXtensible Markup Language) una clase (LevelParser)que permet�a cargar informaci�n mesiante un .ase, esto ayuda
	enormemente en el desarrollo del producto, ya que no solo permite una organizaci�n correcta sino una programaci�n eficiente.

	A continuaci�n, tuvimos que dise�ar la actualizaci�n de los objetos cuando estos realizan algun movimiento. Dicho movimiento, se plante� como un array que poseen waypoints,
	dichos objetos, deber�n seguir en movimiento dichos waypoints. La posici�n del jugador, en cambi�, se guarda automaticamente, sin tener puntos de destino ni nada parecido.

	Mediante el lerp, se consigui� que la camar� hiciera un seguimiento m�s realista de la nave, por ejemplo, al arrancar la camara no la sigue a la misma velocidad, 
	sino que arranca un poco m�s tarde, esto tambi�n ocurre cuando la nave rota.

	Las colisiones fueron una parte muy importante para la jugabilidad, dicha importancia se extrapol� a la dificultad de programarla. Pero a�n as� se consigui�
	dise�ar un sistema mediante las naves enemigas siguiesen a nuestro personaje y dispararan objetos de "bullets" mediante su eficiente array de balas.

	Se program� el Menu system, en el main loop, para que el usuario pudiese recibir informaci�n de "control instruccion", salir del juego, y empezar partida. Enbellecer la estetica
	del videojuego y mejorar la navegabilidad. 
	
	Desarrollamos un sistema de carga y ejecuci�n de audio, lo que aport� una mayor est�tica del videojuego y ayuda a conseguir una mejor interacci�n entre este y el usuario. 
	Asignando una m�sica ambiente y sonidos par los objetos, como por ejemplo los disparos, dan m�s realismo al juego. 

	Finalmente, se program� el Hud, en el que sale un radar con transparencias mientras el juego se est� ejecutando, esta parte se consigui� printar por pantalla,
	tambi�n se hicieron los calculos de las posiciones de las naves enemigas, pero no di� tiempo a poder printarlas correctamente(las posiciones de las naves enemigas).
	
Controles: 
	
	Al inicial el juego, mientras se cargan los objetos aparece una textura de "Loading..." que parpadea
	para que el usuario sepa que se est� cargando el juego y permanezca en espera.
	
	Seguidamente aparece el men� inicial, donde se da la opci�n al usuario de Empezar partida, de ir a opciones 
	(donde podr� visualizar las instrucciones del juego) y de salir del mismo. Para moverse por el men� har� uso de 
	las teclas (?), (?) y (space) barra espaciadora para seleccionar la opci�n.
		
	Los controles con los que podr� interactuar el Jugador s�n los siguientes:

		- (?) bot�n flecha arriba / (w) bot�n "w" : Para que el Jugador pueda avanzar 
			la nave hacia adelante, mantener pulsado para no dejar de avanzar.
		- (?) bot�n flecha abajo / (s) bot�n "s" : Para que el Jugador pueda retroceder
			 la nave hacia atras, mantener pulsado para no dejar de retroceder.
		- (?) bot�n flecha izquierda / (a) bot�n "a" : Con esta tecla el jugador har� 
			 rotar la nave hacia la izquierda, mantener pulsado para no dejar de rotar.
		- (?) bot�n flecha derecha / (d) bot�n "d" : Con esta tecla el jugador har� 
			 rotar la nave hacia la derecha, mantener pulsado para no dejar de rotar.
		- (p) bot�n "p" : Pulsando este bot�n el juego se pausar� momentaneamente, y permite
			 investigar al usuario visualizar el campo de batalla mientras todas las 
			 naves permaneces inamovibles. Para continuar la partida, vuelva a pulsar (p).
		- (space) barra espaciadora: Pulsando (SPACE) el jugador podr� disparar con la nave 
			 proyectiles que seguir�n el puntero laser que hay colocado en la punta de la nave.
	
	Al empezar el juego se avisar� al jugador de que su nave est� en medio de una emboscada.
	En el caso de que pierda, su nave sea destruida, se avisar� mediante un mensaje de "GameOver"

Objetivo: 
	
	El objetivo del videojuego era acercarse lo m�ximo posible la la historia que dise�amos en la fase 1 del proyecto,
	donde concretamos la tem�tica, mec�nicas y el desarrollo del juego.
	
	La historia de nuestro videojuego trata la emocionante y peligrosa vida de un nuevo comerciante, reci�n llegado al
	sector Neo�136 del espacio, que no tiene m�s remedio que dedicarse a la compra y venta de recursos para estaciones espaciales.
	Nuestro protagonista se ver� envuelto peligrosos conflictos con otras naves comerciantes que por intereses econ�micos
	no toleran que haya nuevos competidores en sus rutas comerciales. 
	El personaje principal con el transcurso del videojuego, y gracias a las recompensas obtenidas, de mano de las
	estaciones espaciales por llevar a cabo sus misiones comerciales, podr� mejorar su nave o incluso comprar nuevas. Adem�s, ganar�
	honor y puntos de experiencia.

	La historia contada er� la idea esencial, y aunque la base de los conflictos espaciales si que se han podido desarrollar, por falta de
	tiempo han quedado bastantes puntos a realizar. Objetivos que han faltado por desarrollar:
		- La creaci�n de recursos con valores propios del videojuego, como por ejemplo, que el jugador consiguiera puntos o monedas por nave destruida.
		- Conseguir que el jugador realizara misiones, que utilizando el radar, llegara a cierto lugar del mapa y entregara un paquete. Consiguiendo una
		recompensa por ello.
		- La creaci�n de particulas, en el momento de destrucci�n de una nave, por ejemplo, tampoco se ha podido realizar.
		- Una correcta implementaci�n del HUD, y aunque aparentemente el codigo si calcula las posiciones exactas a peque�a escala
		de las naves enemigas, no han conseguido printarse en el radar con trasparencias, que si que se ha implementado correctamente.

	Los objetivos si realizados correctamente est�n escritos en el apartado "descripci�n" o en el siguiente "Requerimentos implementados".

Requerimientos implementados:
	
	- Requerimientos basicos:
		*Level loading from XML
		*Menu system
		*HUD: Realmente por problemas a la hora de crear el radar lo hemos tenido que 
		      dejar a medias. En el c�digo esta plasmado nuestro intento. 
		*Points System.
		*Bullet Manager
		*Waypoints
		*Load audio
		*Different types of enemy (uno de ellos viene acompa�ado por dos navecitas
		       debes elegir si matar a la nave madre y eliminar a todas o matarlas una a una)
		*Rendering Optimisations: Hemos conseguido optimizar el render de las balas y el c�digo
		       esta plasmado los dos m�todos que hemos utilizado.
				-Pintar bala por bala
				-Pintar todas juntas en una sola malla (La que esta implementada dentro del juego)
		*Different shaders 
	
	- Adem�s de esto hemos implementado una peque�a IA en las naves.En un primer momento algunas de ellas 
	estar�n en movimiento (waypoints) y otras est�ticas. Pero cuando te acerques lo suficente se girar�n
	y te seguir�n apuntandote en todo momento. Creando a si un sistema de waypoints en el cual tu eres 
	el punto al cual llegar.