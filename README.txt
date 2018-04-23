CREADO POR:
	Miguel Ángel Díaz Rodríguez ls31284
	Marc Millán Gimeno ls29307

Proyecto: 
	BLACK OCEAN 

YOUTUBE: https://youtu.be/ojbwyqJy7Bg

Descripcion:

	Para la implementación de este videojuego hemos utilizado Visual Studio, un entorno
	de desarrollo integrado (IDE) para sistemas operativos Windows. Programando en C#.

	Para la producción de este videojuego primero se programó desde practicamente cero un motor 
	gráfico que nos permite leer assets, como objetos diseñados en 3D, en formato .obj, texturas,...
	
	Una vez implemenentada la primera parte que nos permitía cargar "resources", assets mediante su información en el .tga
	y sus texturas, se programo el main loop, que será el puente a poder acceder al Menú principal, la pantalla de espera
	(Loading...), el nivel 1 y el modo de "Pause". También se implementó la clase GameObjectMesh, GameObjectLight, GameObjectEnviroment,
	muy importantes, que contendrá la geometría, textura, material... de cada objecto. Desde ese punto de implementó la herencia de objetos,
	y se cargó una esfera con una textura ambientada en el espacio.
	
	Seguidamente, aparecio la clase Scene, que nos permitió mediante un rootNode, linkear cada uno de los objetos para poder acceder
	comodamente y rapidamente a cada uno de ellos.
	
 	Implementamos también  para  poder utilizar XML (eXtensible Markup Language) una clase (LevelParser)que permetía cargar información mesiante un .ase, esto ayuda
	enormemente en el desarrollo del producto, ya que no solo permite una organización correcta sino una programación eficiente.

	A continuación, tuvimos que diseñar la actualización de los objetos cuando estos realizan algun movimiento. Dicho movimiento, se planteó como un array que poseen waypoints,
	dichos objetos, deberán seguir en movimiento dichos waypoints. La posición del jugador, en cambió, se guarda automaticamente, sin tener puntos de destino ni nada parecido.

	Mediante el lerp, se consiguió que la camará hiciera un seguimiento más realista de la nave, por ejemplo, al arrancar la camara no la sigue a la misma velocidad, 
	sino que arranca un poco más tarde, esto también ocurre cuando la nave rota.

	Las colisiones fueron una parte muy importante para la jugabilidad, dicha importancia se extrapoló a la dificultad de programarla. Pero aún así se consiguió
	diseñar un sistema mediante las naves enemigas siguiesen a nuestro personaje y dispararan objetos de "bullets" mediante su eficiente array de balas.

	Se programó el Menu system, en el main loop, para que el usuario pudiese recibir información de "control instruccion", salir del juego, y empezar partida. Enbellecer la estetica
	del videojuego y mejorar la navegabilidad. 
	
	Desarrollamos un sistema de carga y ejecución de audio, lo que aportó una mayor estética del videojuego y ayuda a conseguir una mejor interacción entre este y el usuario. 
	Asignando una música ambiente y sonidos par los objetos, como por ejemplo los disparos, dan más realismo al juego. 

	Finalmente, se programó el Hud, en el que sale un radar con transparencias mientras el juego se está ejecutando, esta parte se consiguió printar por pantalla,
	también se hicieron los calculos de las posiciones de las naves enemigas, pero no dió tiempo a poder printarlas correctamente(las posiciones de las naves enemigas).
	
Controles: 
	
	Al inicial el juego, mientras se cargan los objetos aparece una textura de "Loading..." que parpadea
	para que el usuario sepa que se está cargando el juego y permanezca en espera.
	
	Seguidamente aparece el menú inicial, donde se da la opción al usuario de Empezar partida, de ir a opciones 
	(donde podrá visualizar las instrucciones del juego) y de salir del mismo. Para moverse por el menú hará uso de 
	las teclas (?), (?) y (space) barra espaciadora para seleccionar la opción.
		
	Los controles con los que podrá interactuar el Jugador són los siguientes:

		- (?) botón flecha arriba / (w) botón "w" : Para que el Jugador pueda avanzar 
			la nave hacia adelante, mantener pulsado para no dejar de avanzar.
		- (?) botón flecha abajo / (s) botón "s" : Para que el Jugador pueda retroceder
			 la nave hacia atras, mantener pulsado para no dejar de retroceder.
		- (?) botón flecha izquierda / (a) botón "a" : Con esta tecla el jugador hará 
			 rotar la nave hacia la izquierda, mantener pulsado para no dejar de rotar.
		- (?) botón flecha derecha / (d) botón "d" : Con esta tecla el jugador hará 
			 rotar la nave hacia la derecha, mantener pulsado para no dejar de rotar.
		- (p) botón "p" : Pulsando este botón el juego se pausará momentaneamente, y permite
			 investigar al usuario visualizar el campo de batalla mientras todas las 
			 naves permaneces inamovibles. Para continuar la partida, vuelva a pulsar (p).
		- (space) barra espaciadora: Pulsando (SPACE) el jugador podrá disparar con la nave 
			 proyectiles que seguirán el puntero laser que hay colocado en la punta de la nave.
	
	Al empezar el juego se avisará al jugador de que su nave está en medio de una emboscada.
	En el caso de que pierda, su nave sea destruida, se avisará mediante un mensaje de "GameOver"

Objetivo: 
	
	El objetivo del videojuego era acercarse lo máximo posible la la historia que diseñamos en la fase 1 del proyecto,
	donde concretamos la temática, mecánicas y el desarrollo del juego.
	
	La historia de nuestro videojuego trata la emocionante y peligrosa vida de un nuevo comerciante, recién llegado al
	sector Neo·136 del espacio, que no tiene más remedio que dedicarse a la compra y venta de recursos para estaciones espaciales.
	Nuestro protagonista se verá envuelto peligrosos conflictos con otras naves comerciantes que por intereses económicos
	no toleran que haya nuevos competidores en sus rutas comerciales. 
	El personaje principal con el transcurso del videojuego, y gracias a las recompensas obtenidas, de mano de las
	estaciones espaciales por llevar a cabo sus misiones comerciales, podrá mejorar su nave o incluso comprar nuevas. Además, ganará
	honor y puntos de experiencia.

	La historia contada erá la idea esencial, y aunque la base de los conflictos espaciales si que se han podido desarrollar, por falta de
	tiempo han quedado bastantes puntos a realizar. Objetivos que han faltado por desarrollar:
		- La creación de recursos con valores propios del videojuego, como por ejemplo, que el jugador consiguiera puntos o monedas por nave destruida.
		- Conseguir que el jugador realizara misiones, que utilizando el radar, llegara a cierto lugar del mapa y entregara un paquete. Consiguiendo una
		recompensa por ello.
		- La creación de particulas, en el momento de destrucción de una nave, por ejemplo, tampoco se ha podido realizar.
		- Una correcta implementación del HUD, y aunque aparentemente el codigo si calcula las posiciones exactas a pequeña escala
		de las naves enemigas, no han conseguido printarse en el radar con trasparencias, que si que se ha implementado correctamente.

	Los objetivos si realizados correctamente están escritos en el apartado "descripción" o en el siguiente "Requerimentos implementados".

Requerimientos implementados:
	
	- Requerimientos basicos:
		*Level loading from XML
		*Menu system
		*HUD: Realmente por problemas a la hora de crear el radar lo hemos tenido que 
		      dejar a medias. En el código esta plasmado nuestro intento. 
		*Points System.
		*Bullet Manager
		*Waypoints
		*Load audio
		*Different types of enemy (uno de ellos viene acompañado por dos navecitas
		       debes elegir si matar a la nave madre y eliminar a todas o matarlas una a una)
		*Rendering Optimisations: Hemos conseguido optimizar el render de las balas y el código
		       esta plasmado los dos métodos que hemos utilizado.
				-Pintar bala por bala
				-Pintar todas juntas en una sola malla (La que esta implementada dentro del juego)
		*Different shaders 
	
	- Además de esto hemos implementado una pequeña IA en las naves.En un primer momento algunas de ellas 
	estarán en movimiento (waypoints) y otras estáticas. Pero cuando te acerques lo suficente se girarán
	y te seguirán apuntandote en todo momento. Creando a si un sistema de waypoints en el cual tu eres 
	el punto al cual llegar.