<h1 align="center">IITD x Pacman</h1>
<p align="center">
  A two player pacman-ish maze game built as part of the course COP290 under Prof. Rijurekha Sen<br>
  Authors : <a href="https://github.com/Harshita2605">Harshita</a> & <a href="https://github.com/omkrishna">Om</a></p>
  
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


<h2> Follow Up </h2>
<h3> <a href=""> Playing Guide </a></h3>
<h3> <a href=""> Roadmap </a> </h3>

<h2>Acknowledgments</h2>
<ul>
  <li> <a href="">Lazy Foo</a>'s sdl tutorials are a must watch for anyone who wants to try out SDL. Music, Textures, Fonts, TileMaps - this tutorial has everything covered as far as SDL is concerned. </li>
  <li> <a href="https://shaunlebron.github.io/pacman-mazegen/"> Shaun Lebron</a>'s article on Random Pacman maze generation was a very tough read but gives excellent results. We couldn't follow his code but did get the idea of stacking tetris-like blocks together to generate mazes. </li>
  <li> <a href="https://www.youtube.com/watch?v=ataGotQ7ir8">This Youtube video</a> gives a great insight about the algorithms ghosts follow to set their targets in pacman. Very fascinating</li>
</ul>



