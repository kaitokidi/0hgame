#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

int main(){
    
    //VARIABLES CONSTANTS i ENUMERATIONS
    int speed = 180;
    float time_to_next_sprite = 0.09;
    enum direction { down, left, right, up , none};
    const int mx[5] = {0,   -1,   1,    0,      0};
    const int my[5] = {1,    0,   0,   -1,      0};
    
    //VECTOR2
    sf::Vector2u size(1100,700);
    sf::Vector2f spriteSize(32,48);
    sf::Vector2i spriteSource(0,down);
    sf::Vector2f originalSpriteSize(0,0);
    sf::Vector2u newsize(size.x, size.y);
    sf::Vector2f playerPosition(spriteSize.x,spriteSize.y);
    
    //OBJECTES DE SFML
        //DECLAREM LA VIEW
    sf::View view;
    sf::Image image;
    sf::Event event;
    sf::Clock deltaClock;
    sf::Sprite player, background;
    sf::Texture pTexture, tbackground;
    
    //LOAD THE IMAGES AND TEXTURES
    if(!image.loadFromFile("ground.png")) std::cout << "ground Image Not Loaded " << std::endl;
    if(!tbackground.loadFromImage(image)) std::cout << "background texture Not Loaded " << std::endl;
    if(!pTexture.loadFromFile("sprites.png")) std::cout << "personatge Not Loaded " << std::endl;
    spriteSize.x = originalSpriteSize.x = pTexture.getSize().x/4;
    spriteSize.y = originalSpriteSize.y = pTexture.getSize().y/4;
    
    //VARIABLES
    direction d = none;
    double escalat = 1;
    float distx = spriteSize.x/3, disty = spriteSize.y/4;
    float deltatime = 0, movx = 0.0, movy = 0.0, scont = 0.0;
    
    //Set player's properties
    player.setTexture(pTexture);
    player.setPosition(playerPosition);
    //Set Background's properties
    background.setTexture(tbackground);
    
    //CREATE THE WINDOW
    sf::RenderWindow window(sf::VideoMode(size.x,size.y), "Penguins <3", sf::Style::Resize|sf::Style::Close);

    //GAME LOOP
    while(window.isOpen()){
        speed = 180*((window.getSize().x - playerPosition.x)/window.getSize().x)+20;
        //Loop for handling events
        while(window.pollEvent(event)){
            switch (event.type){
                //Close event
                case sf::Event::Closed:
                    window.close();
                    break;
                    //KeyPressed event
                case sf::Event::KeyPressed:
                    //Close key
                    if (event.key.code == sf::Keyboard::Escape) {
                        window.close();
                    }
                    break;
                    //Default
                default:
                    //Do nothing
                    break;
            }
        }   
        
        //Deltatime
        deltatime = deltaClock.restart().asSeconds();
        scont += deltatime;
        
        //Updating if a key is pressed in a direction 'd'
        d = none;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) d = up;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))  d = down;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) d = right;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))  d = left;
        
        //If some key is pressed
        if(d != none){
            //Set value to movement variables and update spritesource
            if(spriteSource.y == d){
                movx += mx[d]*speed*deltatime;
                movy += my[d]*speed*deltatime;
            }
            else spriteSource.y = d;
			if (scont >= time_to_next_sprite){
				scont = 0;
				++spriteSource.x;
			}
        }
        
        //Calcula el desti del player
        float destix = playerPosition.x+movx;
        float destiy = playerPosition.y+movy;
    
        //Calcula l'equivalent del punt de la pantalla on esta el player en la imatge de background
		float px, py;
		if(destix > 0 && destiy > 0) {
			px = image.getSize().x*destix/tbackground.getSize().x/background.getScale().x;
			py = image.getSize().y*destiy/tbackground.getSize().y/background.getScale().y;
			
	       		if(image.getPixel(px,py) != sf::Color::Black) {
				playerPosition.x = destix;
				playerPosition.y = destiy;
			}
			else {
				playerPosition.x = 1;
				playerPosition.y = 1;
			}
		}
		else {
			playerPosition.x = 1;
			playerPosition.y = 1;
		}
        
        //Setting movement variables to 0
        movx = movy = 0;
        
        //Checking the sprite sources and position to be ok
        if(spriteSource.x >= 4) spriteSource.x = 0;
        float aux = background.getTexture()->getSize().y * background.getScale().y;
        if(playerPosition.x < 0) playerPosition.x = 0;
        else if (playerPosition.x >= window.getSize().x-3) { 
			if(!pTexture.loadFromFile("sprites3.png")) std::cout << "personatge Not Loaded " << std::endl;
			if(!image.loadFromFile("ground2.png")) std::cout << "ground Image Not Loaded " << std::endl;
			if(!tbackground.loadFromImage(image)) std::cout << "background texture Not Loaded " << std::endl;
			playerPosition.x = 1;	playerPosition.y = 1;
		}
        else if (playerPosition.y < 0) playerPosition.y = 0;
        else if (playerPosition.y > aux) {
			playerPosition.x = 1;   playerPosition.y = 1;
		}
        
        
        //Set player properties
        player.setOrigin(spriteSize.x/2, spriteSize.y);
        player.setPosition(playerPosition.x, playerPosition.y);
        player.setTextureRect(sf::IntRect(spriteSource.x*spriteSize.x,
                                        spriteSource.y*spriteSize.y, spriteSize.x, spriteSize.y));
       // player.setScale(window.getSize().x/600.0, window.getSize().x/600.0);
		player.setScale((window.getSize().x - playerPosition.x)/window.getSize().x , (window.getSize().x - playerPosition.x)/window.getSize().x);
        player.scale(originalSpriteSize.x/spriteSize.x, originalSpriteSize.y/spriteSize.y);
        
        //Set background scale
        escalat = (double)(window.getSize().x) / (double)(tbackground.getSize().x);
        background.setScale(escalat, escalat);
        
        //Set VIEW values
        view.reset(sf::FloatRect(playerPosition.x,playerPosition.y, window.getSize().x, window.getSize().y));
        //Definim on volem centrar la VIEW
        view.setCenter(playerPosition);
   
//         view.setRotation(playerPosition.y+playerPosition.x);
//         view.zoom((view.getSize().y/2) / playerPosition.y);
		view.zoom(0.4);
        //view.setViewport(sf::FloatRect(0,0,1,1));
        //Clear the window
        window.clear();
        //Set window view, draw and display
        window.draw(background);
        window.draw(player);
        window.setView(view);
        window.display();
    }
}
