# RaycasterDemo
This live demonstation shows the process of projecting a 2D map to a first person optical field of view, or FOV. In game development this is also known as first person. 

## Demo
![](images/screenrecording.gif)
The left half of this demo displays the 2D birds eye view of a 16x16 grip map as well as the players position, and the raycasting done to determine the players distance from walls in the field of view. The right half of this demo displays the first person field of view from the players perspective. 



## Building the Project 
1. Clone this repo:
    ```
    git clone https://github.com/denglish04/raycasterDemo
    ```

2. Navigate to the project folder
    ```
    cd RaycasterDemo/src
    ```

3. Compile using GCC compiler and sdl2 configs 
    ```
    g++ gameloop.cpp -o gameloop $(sdl2-config --cflags --libs)
    ```
    or for windows
    ```
    g++ gameloop.cpp -o gameloop -lmingw32 -lSDL2main -lSDL2

    ```

    1. If the GCC compiler is not install 
    ```
    brew install gcc
    ```

5. Run 
    ```
    ./gameloop
    ```


## Screenshots
![](images/screenshot1.png)    
![](images/screenshot2.png)    
![](images/screenshot3.png)    




