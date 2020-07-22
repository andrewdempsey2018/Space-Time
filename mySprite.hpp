#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP
#include <SFML/Graphics.hpp>
#endif

#ifndef VECTOR_HPP
#define VECTOR_HPP
#include <vector>
#endif

class mySprite
{
    public:

        mySprite(float, float, float, bool, int, int, int);
        void move(float);
        void reverse();
        void kill();
        void animate(int);

        float xPos, yPos, speed;
        int width, height, frames;
        bool alive, flipped, goingLeft;
        sf::Sprite sprite;

        float animationStep;
        short frameNumber;

        vector<sf::IntRect> spriteRects;
};

mySprite::mySprite(float x, float y, float spd, bool a, int w, int h, int fra)
{
    xPos = x;
    yPos = y;
    speed = spd;
    alive = a;
    width = w;
    height = h;
    frames = fra;

    animationStep = 0.0;
    frameNumber = 0;

    for(int i = 0; i < frames; i++)
    {
        spriteRects.push_back(sf::IntRect(width * i, 0, width, height));
    }

    sprite.setTextureRect(spriteRects.at(0));

    flipped = true;
    goingLeft = true;
}

void mySprite::move(float interpolValue)
{
    //xPos += speed * dlt;
    //sprite.setPosition(xPos + (speed * interpolValue), yPos);
    //vertPlatformSprite.setPosition(vertPlatformX, vertPlatformY + (vertPlatformSpeed * interpolation));
}

void mySprite::reverse()
{
    //speed = speed * -1;
    //flipped = !flipped;
    // flip X
    //sprite.setTextureRect(sf::IntRect(width, 0, -width, height));
}

void mySprite::kill()
{
    alive = false;
    xPos = -500.0f;
    yPos = -500.0f;
    frameNumber = 0;
    animationStep = 0.0;
    sprite.setPosition(xPos, yPos);
}

void mySprite::animate(int frame)
{
    if(flipped)
    {
        sprite.setTextureRect(spriteRects.at(frame));
    }

    //if(!flipped)
    //{
        //sprite.setTextureRect(spriteRects[frame + (frames / 2)]);
    //}
}

