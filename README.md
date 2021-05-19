<h1 align="center">IITD x Pacman</h1>
<p align="center">
  A two player pacman-ish maze game built as part of the course COP290 under Prof. Rijurekha Sen<br>
  Authors : <a href="https://github.com/Harshita2605">Harshita</a> & <a href="https://github.com/omkrishna">Om</a></p>
  
  ![ezgif com-gif-maker (4)](https://user-images.githubusercontent.com/10244292/118809280-0a420180-b8c8-11eb-9b9c-dd6e5088a006.gif)

  
<h2>Getting Started</h2>
<h3>Dependencies</h3>
<ul>
  <li>G++, which is a C++ compiler </li>
  <li>SDL 2.0 (We go through installing SDL on a ubuntu system, and give links for other OSs </li>
</ul>
<h3>Installing</h3>
Assuming you have g++ installed, here's how to install the sdl libraries on a ubuntu system : 

```
#install sdl2
sudo apt install libsdl2-dev libsdl2-2.0-0 -y;

#install sdl image  
sudo apt install libjpeg-dev libwebp-dev libtiff5-dev libsdl2-image-dev libsdl2-image-2.0-0 -y;

#install sdl mixer  
sudo apt install libmikmod-dev libfishsound1-dev libsmpeg-dev liboggz2-dev libflac-dev libfluidsynth-dev libsdl2-mixer-dev libsdl2-mixer-2.0-0 -y;

#install sdl fonts
sudo apt install libfreetype6-dev libsdl2-ttf-dev libsdl2-ttf-2.0-0 -y;
```

<a href="https://lazyfoo.net/tutorials/SDL/01_hello_SDL/index.php">Link for other OSs </a> 
<h3>Running the game</h3>

```
git clone https://github.com/omkrishna/COP290-TASK2.git
cd PacMan
make
```

<h2>Game Logic</h2>

<h3>Character Logic</h3>
The Game has two main <i>characters - students PacMan and PacWoman</i>. They move around the maze taking inputs from the keyboard - Arrow Keys in single player mode and Arrow Keys and WSAD KeySet in two-player mode. In single player mode, the game randomly chooses the student as PacMan or PacWoman, who initially stands at the top left empty tile. In two-player mode, PacMan and PacWoman randomly occupy one of the top-right and top-left empty tiles. The Students score by picking up Pellets, PoRs and Audits. A Student wins if they collect all the pellets before failing. <i>The users cannot pause the game because Students at IITD never get a break</i>.

<h3>Ghost Logic</h3>
  The Game has <i>3 Ghosts - courses COP, ELL and HUL</i>. They move around the maze following agorithms computed in real-time. The Ghosts cannot step on Tiles and never take an about turn. The courses calculate the distance to their target tiles from the available places they can move to, and shift over to the place with the minimum distance. Whenever a Course collides with a Student, the Student is deemed <i>failed</i> and stops taking keyboard inputs. The speed of the Courses - <i>the WorkLoad</i> - is decided at the welcome screen, where the user choses their level - beginner, intermediate or advanced. 
<br><br>
In single player mode, COP's target tile is the Student itself. HUL's target tile is 2 tiles ahead of the Student. And ELL's target tile takes into account the positions of the Student, COP and HUL, owing to its unpredictable and erratic behavior. It lies in the same line as COP and HUL's target tiles such that HUL's target tile is in the exact middle. However, in multiplayer mode, COP and HUL each chase one Student and ELL roams with respect to the positions of COP, HUL and the two Students. If one Student fails, all Courses turn to <i>Relative Grading</i> and chase the remaining Student.
<br><br>
  In case a Student picks up a PoR Pellet, the Courses enter into a <i>Backlog Mode</i> for a few seconds, i.e. all their target tiles are set to their respective corners in the screen - top-right, bottom-right and bottom-left for COP, HUL and ELL respectively. In the event that a Student picks up an Audit Pellet, the Courses enter into <i>30% Effort mode</i>. Their target tile is the usual, but when they get in the radius of 8 tiles around the Student(s), they idle out and target tile shifts to their respective corners. When all Students in the game have failed, the exit animation pops up where the Courses do <i>a little victory dance</i> (by moving randomly irrespective of the other sprites) before the exit screen. Well, they do that even if a Student manages to win, cause does a Student really win?

<h3>Maze Logic</h3>
--maze generation logic
--easter egg

--tiles and pellets setting, collision with tiles vs pellets 


<h2> Follow Up </h2>
<h3> <a href=""> Playing Guide </a></h3>
<h3> <a href=""> Roadmap </a> </h3>

<h2>Acknowledgments</h2>
<ul>
  <li> <a href="">Lazy Foo</a>'s sdl tutorials are a must watch for anyone who wants to try out SDL. Music, Textures, Fonts, TileMaps - this tutorial has everything covered as far as SDL is concerned. </li>
  <li> <a href="https://shaunlebron.github.io/pacman-mazegen/"> Shaun Lebron</a>'s article on Random Pacman maze generation was a very tough read but gives excellent results. We couldn't follow his code but did get the idea of stacking tetris-like blocks together to generate mazes. </li>
  <li> <a href="https://www.youtube.com/watch?v=ataGotQ7ir8">This Youtube video</a> gives a great insight about the algorithms ghosts follow to set their targets in pacman. Very fascinating</li>
</ul>



