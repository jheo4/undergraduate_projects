---
layout: default
---
[back](./pygame){: .btn.btn-default}

# The Basis for Jin Fighting Game

For making a game, I don't recommend that someone imagines big commercial games. When I imagine the huge scale games, there are too many things to handle. Therefore, it is easy to be lost. In this page, I only focus on making a basis of a game, and introduce PyGame lib functions. 

To install PyGame, visit [PyGame official website](https://www.pygame.org/download.shtml){: target="_blank"}. I strongly recommend to use [pip](https://pypi.python.org/pypi/pip){: target="_blank"} to install PyGame. I also use [PyCharm](https://www.jetbrains.com/pycharm/){: target="_blank"} with PyGame to make my own game. After installing these programs, Congratulation! It's time to get the ball rolling.

pyGameProject
- main.py
- GameLoop.py
- units
	- __init__.py
	- UnitBase.py
	- Character.py
	- Structure.py
- resources
	- images
		- mainMenu1.png
		- mainMenu2.png
		- playerDump.png

<br />

My project structure is like this; main.py is the main menu loop of the game, and GameLoop is only for a game loop. The package "units" is for having whole characters and structures. "resources" has some resources for the game such as images, effect sounds, and background sounds.


	import pygame
	pygame.init()
	wndSize = (800, 600)
	wndTitle = "Jin Game Title"
	
	pygame.display.set_caption(wndTitle)
	menuScreen = pygame.display.set_mode(wndSize)

	isEnd = False
	gameLoop = GameLoop()
	menuImg1 = pygame.image.load("resources/images/mainMenu1.png")
	menuImg2 = pygame.image.load("resources/images/mainMenu2.png")

	menuClock = pygame.time.Clock()
	pygame.time.set_timer(USEREVENT+1, 500)
	isBlinking = False

To use PyGame, these codes must be written in main.py. After importing pygame, the init() function should be called. I also set wndSize, and the caption for the title bar. the isEnd flag is used to know the end of the game. I also make an instance of GameLoop class. the menuImage1 and 2 are used to blink the main menu screen of this game. When I play some consol games, the very fist screen is blinking with the message like "Press G to Start the Game". After loading images, I make a clock for the menu loop, and set a timer to blink the screen.


	while not isEnd:
	  for event in pygame.event.get():
	    # Exit...
	    if (event.type == pygame.QUIT) or (event.type == pygame.KEYDOWN and event.key == pygame.K_q):
	      isEnd = True

	    # Game Start
	    if event.type == pygame.KEYDOWN and event.key == pygame.K_g:
	      # turn MenuLoop timer off
	      pygame.time.set_timer(USEREVENT + 1, 0)

	      # Game loop start!
	      gameLoop.startGame()

	      # turn MenuLoop timer on
	      pygame.time.set_timer(USEREVENT + 1, 500)

	    # Blinking Flag
	    if event.type == USEREVENT+1 :
	      isBlinking = not isBlinking


	  menuScreen.fill((0, 0, 0))
	  # Blinking Timer
	  if isBlinking:
	    menuScreen.blit(menuImg1, [0, 0])
	  else:
	    menuScreen.blit(menuImg2, [0, 0])

	  pygame.display.flip()
	  menuClock.tick(60)

This loop is the main menu loop. The event is used to know key strokes, and the if-statement codes are executied with strokes. To quit this game, press 'q' or close the window. To start this game, press 'g'. The set_timer functions back-and-forth startGame() are to stop/start blinking when the game start/stop. I fill the backgraound screen with black color, and draw images by using the blit function. flip() is to refresh the screen, and tick() is to set the clock basis.

	class GameLoop:
	  isGameLoopEnd = False

	  gameScreen = pygame.display.set_mode((800, 600))
	  gameTimer = pygame.time.Clock()
	  player = Character(_2Dpoint=[100, 100], _hp=100, _size=[10, 10], _moveSpeed=3)


	  def startGame(self):
	    # End flag
	    self.isGameLoopEnd = False

	    # Before Game, Show how to play this game...

	    # initGame ...

	    playerImg = pygame.image.load("resources/images/playerDump.png")

This is a game loop class. In a game loop, processes for drawing the screen, handling events, and update game states are the main tasks. To do these tasks, it is necessary to prepare some resources and options to run the game loop. I only load a player dump image since this is a basis of my future game.

	    # Game Loop...
	    while not self.isGameLoopEnd :
	      for event in pygame.event.get() :
	        if event.type == pygame.KEYDOWN and event.key == pygame.K_ESCAPE: # Exit Game...
	          self.isGameLoopEnd = True

	      if pygame.key.get_pressed()[pygame.K_UP] or pygame.key.get_pressed()[pygame.K_LEFT] or pygame.key.get_pressed()[pygame.K_DOWN] or pygame.key.get_pressed()[pygame.K_RIGHT]:
	        self.player.move()

	      self.gameScreen.fill((0, 0, 0))
	      #Draw...
	      self.gameScreen.blit(playerImg, self.player.get2Dpoint())
	      pygame.display.flip()
	      self.gameTimer.tick(60)

In the game loop, press 'ESC' to quit it. get_pressed() is used to know the moving keys are pressed or not. This application is only focus on making the basis of game, so the main menu loop and game loop with simple fucntions are implemented. The package 'units' has nothing special now, and can be checked by the whole source code github. I'm pretty sure that it is easy to understand.

The full source codes are available [here](https://github.com/jin993/jinFightingGame/){: target="_blank"}