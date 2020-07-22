#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sstream>
#include <string>
#include <fstream>
#include <vector>
using namespace std;

#include "mySprite.hpp"

//gorey code 07-wx-11130

//prototypes
string toString(int);
string toString(float);
void LoadLevel(string level);
void updateGameplay();
void drawGameplay();
bool Collision(sf::Sprite, sf::Sprite);
bool vectorInSprite(sf::Vector2f, sf::Sprite);
void updateTitleScreen();
void drawTitleScreen();
void updateIntro();
void drawIntro();
void resetVars();
void toggleFullScreen();
void toggleVSync();
void toggleFPS();
void makeAllTilesWhite();
void updateGoodEnding();
void drawGoodEnding();
void updateBadEnding();
void drawBadEnding();

sf::RenderWindow game1;

sf::Clock aClock;

const int TICKS_PER_SECOND = 60;
const int SKIP_TICKS = 1000 / TICKS_PER_SECOND;
const int MAX_FRAMESKIP = 5;

long next_game_tick = aClock.getElapsedTime().asMilliseconds();
int loops = 0;

float interpolation = 0.0;
float over_all_game_speed = 1.0;

const int SCREEN_WIDTH_PIXELS = 800;
const int SCREEN_HEIGHT_PIXELS = 600;

//level setup
const int NUMBER_OF_TILE_IMAGES = 34;
const int TILES_ACROSS = 320;
const int TILES_DOWN = 45;
const int TILE_WIDTH = 40;
const int TILE_HEIGHT = 40;
const int NUMBER_OF_LAYERS = 4;
const int NUMBER_OF_TILES = TILES_ACROSS * TILES_DOWN * NUMBER_OF_LAYERS;

//layer1 bg behind
//layer2 collision layer
//layer3 bg forground
//layer4 place enemys / ai for enemys

int layer1[TILES_ACROSS][TILES_DOWN];
int layer2[TILES_ACROSS][TILES_DOWN];
int layer3[TILES_ACROSS][TILES_DOWN];
int layer4[TILES_ACROSS][TILES_DOWN];

sf::Texture tileImages[NUMBER_OF_TILE_IMAGES];

sf::Texture platformTexture;
sf::Sprite horPlatformSprite;
sf::Sprite vertPlatformSprite;

float horPlatformX = -100.0;
float horPlatformY = -220.0;
float horPlatformSpeed = 1.0;
bool onHorPlatform = false;

float vertPlatformX = -200.0;
float vertPlatformY = -210.0;
float vertPlatformSpeed = 1.0;
bool onVertPlatform = false;

//for moving the camera
float offsetX = -400.0;
float offsetY = -300.0;

//main sprite
sf::Texture playerTexture;
sf::Sprite playerSprite;
const int NUMBER_OF_PLAYER_SPRITE_FRAMES = 6;
sf::IntRect playerFrames[NUMBER_OF_PLAYER_SPRITE_FRAMES];

short playerFrameNumber = 0;
short PLAYER_ANIMATION_SPEED = 9.0;
float animationStep = 0.0;

float playerX = 40.0;
float playerY = 40.0;
float playerSpeedX = 0.0;
float playerSpeedY = 0.0;
const float SPEED_UP_AMOUNT = 0.125f;
//const float SLOW_DOWN_AMOUNT = 0.025f;
const float SLOW_DOWN_AMOUNT = 0.125f;
const float MAX_X_SPEED = 5.0;
const float MAX_Y_SPEED = 5.0;
const float GRAVITY = 0.25f;
const float JUMP_HEIGHT = -5.0; //negative number so player jumps up
float extraJumpHeight = 0.0; //gives the player an extra bit of height depending on running speed
bool facingRight = true;
bool jumping = true;
const int PLAYER_WIDTH = 40;
const int PLAYER_HEIGHT = 60;
int maxMoveUp = 0;
int maxMoveDown = 0;
int maxMoveLeft = 0;
int maxMoveRight = 0;
bool point1Collision = false;
bool point2Collision = false;
bool point3Collision = false;
bool point4Collision = false;
bool point5Collision = false;
bool point6Collision = false;
bool point5ACollision = false;
bool point6ACollision = false;
bool point7Collision = false;
bool point8Collision = false;

sf::Vector2f point1, point2, point3, point4, point5, point6, point7, point8, point5A, point6A;

//sfx
sf::SoundBuffer jumpSoundBuffer;
sf::Sound jumpSound;

sf::SoundBuffer headhitSoundBuffer;
sf::Sound headhitSound;
bool canPlayHeadHitSound = true;

sf::SoundBuffer hitenemySoundBuffer;
sf::Sound hitenemySound;

sf::SoundBuffer keytypeSoundBuffer;
sf::Sound keytypeSound;

sf::SoundBuffer pickupSoundBuffer;
sf::Sound pickupSound;

sf::SoundBuffer hurtSoundBuffer;
sf::Sound hurtSound;

sf::SoundBuffer footstepSoundBuffer;
sf::Sound footstepSound;
bool playFootSound = true;

sf::SoundBuffer firstwarningSoundBuffer;
sf::Sound firstwarningSound;
bool firstwarningPlayed = false;

sf::SoundBuffer secondwarningSoundBuffer;
sf::Sound secondwarningSound;
bool secondwarningPlayed = false;

sf::SoundBuffer thirdwarningSoundBuffer;
sf::Sound thirdwarningSound;
bool thirdwarningPlayed = false;

sf::SoundBuffer fourthwarningSoundBuffer;
sf::Sound fourthwarningSound;
bool fourthwarningPlayed = false;

sf::SoundBuffer exitSoundBuffer;
sf::Sound exitSound;

struct block
{
    sf::Sprite sprite;
    float x;
    float y;
};

block blocks[NUMBER_OF_TILES];

int drawBlocksCounter = 0;

bool escapePressed = false;

int loadLevelCounter = 0;

//3 explosions for enemy kill
const short NUMBER_OF_EXPLOSIONS = 3;
sf::Texture explosionTexture;
vector<mySprite> explosion(NUMBER_OF_EXPLOSIONS, mySprite(-500.0, -500.0, 2.0, false, 40, 40, 8));
const short EXPLOSION_ANIMATION_SPEED = 9.0;
short whichExplosionToDisplay = 0;

//5 collection animations
const short NUMBER_OF_COLLECTION_ANIMATIONS = 5;
sf::Texture collectionTexture;
vector<mySprite> collection(NUMBER_OF_COLLECTION_ANIMATIONS, mySprite(-500.0, -500.0, 2.0, false, 40, 40, 6));
const short COLLECTION_ANIMATION_SPEED = 9.0;
short whichCollectionToDisplay = 0;

//background color
short color1 = 227;
short color2 = 105;
short color3 = 2;

//music tests
sf::Music inGameMusic;
sf::Music titleScrMusic;

//sprites
//enemys
const int NUMBER_OF_EACH_ENEMY = 100;
sf::Texture enemy1Texture;
vector<mySprite> enemy1s(NUMBER_OF_EACH_ENEMY, mySprite(150.0, 150.0, 2.0, false, 40, 40, 4));
float enemy1AnimationStep = 0.0;
short ENEMY1_ANIMATION_SPEED = 9.0;
short enemy1FrameNumber = 0;
short enemy1Counter = -1; //used for loading sprites into the level

//pickups
const int NUMBER_OF_PICKUPS = 100;
sf::Texture pickupTexture;
vector<mySprite> pickups(NUMBER_OF_PICKUPS, mySprite(-500.0, -500.0, 2.0, false, 40, 40, 7));
float pickupAnimationStep = 0.0;
short PICKUP_ANIMATION_SPEED = 20.0;
short pickupFrameNumber = 0;
short pickupCounter = -1; //used for loading pickups into the level

//level list
vector<string> levelListVector;
int levelNumber = -1;
bool tabPressed = false;

//exit level sprite
mySprite levelExit(-500.0, -500.0, 2.0, false, 40, 40, 7);
sf::Texture levelExitTexture;
int timeBeforeNextLevelLoads = 120;

//for managing gamestates
enum state
{
    gamePlay,
    titleScreen,
    intro,
    goodEnding,
    badEnding
};

state gameState = titleScreen;

const short LAST_LEVEL_NUMBER = 10; //change to last level when level build is complete

//anomaly intensity
enum anomIntensity
{
    none,
    high,
    medium,
    low
};

anomIntensity anomelyIntensity = none;

int effectChange = 0; //slows down the rate of change of the anonely effects
sf::Color changeTilesToColor = sf::Color::White;

bool returnPressed = false;

//for use on the title screen
struct square
{
    sf::Color color;
    float size;
    float horSpeed;
    float vertSpeed;
    float xPos;
    float yPos;
    bool active;
    sf::RectangleShape sprite;
};

struct line
{
    float yPos;
    bool active;
    sf::RectangleShape sprite;
};

const short NUMBER_OF_LINES = 50;

line lines[NUMBER_OF_LINES];

const sf::Color LINE_COLOR = sf::Color::White;
const float LINE_X_POS = 40.0;
const float LINE_SPEED = 0.5;
const float LINE_WIDTH = 720.0;
const float LINE_HEIGHT = 1.0;

//text
sf::Font atariFont;

sf::Text pressEnterText;
sf::Text pressFText;
sf::Text pressEscapeText;
sf::Text countDownTimerText;

sf::Text diagonaluniverseText;

//ingame menu text
sf::Text gmenu_gamePausedText, gmenu_pressEscText, gmenu_qForTitleText, gmenu_fToggleFScreen, gmenu_vToggleVsync;
sf::Text gmenu_sChangeFPS, gmenu_fpsdisplay, gmenu_vsyncdisplay, gmenu_fullscreendisplay, tmenu_menuText;
sf::Text tmenu_pressEscText, tmenu_qForOSText;

//for toggling screen modes
bool fPressed = false;
bool qPressed = false;
bool vPressed = false;
bool sPressed = false;
bool fullScreenMode = false;
int frameLimit = 70;
bool vSyncOn = false;

//for use on the intro
int amountOfTextToShow = 0;
bool finishedText = false;
int textProgressor = 0;
short slowDownText = 0;

string introString =
"Hello there.\n"
"I am your creator.\n"
"I am also the creator of your world.\n"
"I made you perfect,\n"
"but your world has errors.\n"
"I command you to fix these errors\n"
"before the space time\n"
"continuum collapses.\n"
"[Your space time continuum anyway].9";

const int NUMBER_OF_TEXT_LINES = 9;
sf::Text textLines[NUMBER_OF_TEXT_LINES];
int lineNumber = 0;
int lineStart = 0;

//endings

//for use on the good ending
int amountOfTextToShowGE = 0;
bool finishedTextGE = false;
int textProgressorGE = 0;
short slowDownTextGE = 0;

string goodEndingString =
"Exelent!\n"
"You have fixed all of the errors!\n"
"You have worked better than I expected.\n"
"I'll reward you by making you human.4";

const int NUMBER_OF_TEXT_LINESGE = 4;
sf::Text textLinesGE[NUMBER_OF_TEXT_LINESGE];
int lineNumberGE = 0;
int lineStartGE = 0;

int endingRobotOpacity = 255;
int endingHumanOpacity = 0;

sf::Sprite endingRobotSprite;
sf::Texture endingRobotTexture;
sf::Sprite endingHumanSprite;
sf::Texture endingHumanTexture;

const int TOTAL_ENEMYS_IN_GAME = 100;
int numberOfEnemysKilledByPlayer = 0;
int tempHolderForPlayerScore = 0;

sf::Text results;

//for use on the bad ending
int amountOfTextToShowBE = 0;
bool finishedTextBE = false;
int textProgressorBE = 0;
short slowDownTextBE = 0;

string badEndingString =
"Good work!\n"
"You have managed to fix many errors\n"
"Your world is alot better\n"
"I can delete you now.4";

const int NUMBER_OF_TEXT_LINESBE = 4;
sf::Text textLinesBE[NUMBER_OF_TEXT_LINESBE];
int lineNumberBE = 0;
int lineStartBE = 0;

const string GAME_NAME = "Space Time";

int secondsCounter = 0;
const int COUNTDOWN_TIMER_RESET = 10;
int countDownTimer = COUNTDOWN_TIMER_RESET;
sf::RectangleShape countDownTimerBg;

//for pausing the game when the player gets hurt
short playerHurt = 0;
short invincibleTime = 0;

//for in game menu and title screen menu
sf::RectangleShape menuBox;
bool showInGameMenu = false;
bool showTitleScreenMenu = false;

bool exitingLevel = false;

//death
bool dead = false;
int short deadTimer = 0;

int main()
{
    //game1.create(sf::VideoMode(SCREEN_WIDTH_PIXELS, SCREEN_HEIGHT_PIXELS), GAME_NAME, sf::Style::Fullscreen);
    game1.create(sf::VideoMode(SCREEN_WIDTH_PIXELS, SCREEN_HEIGHT_PIXELS), GAME_NAME, sf::Style::Close);

    game1.setMouseCursorVisible(false);

    sf::Event event;

    //display options
    game1.setFramerateLimit(frameLimit);
    game1.setVerticalSyncEnabled(vSyncOn);

    //initialise on screen text
    for(int i = 0; i < NUMBER_OF_TEXT_LINES; i++)
    {
        textLines[i].setFont(atariFont);
        textLines[i].setCharacterSize(20);
        textLines[i].setColor(sf::Color::White);
        textLines[i].setPosition(20, (i * 30) + 20);
    }

    for(int i = 0; i < NUMBER_OF_TEXT_LINESGE; i++)
    {
        textLinesGE[i].setFont(atariFont);
        textLinesGE[i].setCharacterSize(20);
        textLinesGE[i].setColor(sf::Color::White);
        textLinesGE[i].setPosition(20, (i * 30) + 20);
    }

    for(int i = 0; i < NUMBER_OF_TEXT_LINESBE; i++)
    {
        textLinesBE[i].setFont(atariFont);
        textLinesBE[i].setCharacterSize(20);
        textLinesBE[i].setColor(sf::Color::White);
        textLinesBE[i].setPosition(20, (i * 30) + 20);
    }

    results.setFont(atariFont);
    results.setCharacterSize(20);
    results.setColor(sf::Color::Red);
    results.setPosition(100, 220);

    //enemys
    //enemy1 texture
    if(!enemy1Texture.loadFromFile("files\\sprites\\enemy1.png"))
    {
        return EXIT_FAILURE;
    }

    for(int i = 0; i < NUMBER_OF_EACH_ENEMY; i++)
    {
        enemy1s.at(i).sprite.setTexture(enemy1Texture);
    }

    //pickup graphics
    if(!pickupTexture.loadFromFile("files\\sprites\\pickup.png"))
    {
        return EXIT_FAILURE;
    }

    for(int i = 0; i < NUMBER_OF_PICKUPS; i++)
    {
        pickups.at(i).sprite.setTexture(pickupTexture);
    }

    //exit texture and sprite
    if(!levelExitTexture.loadFromFile("files\\sprites\\exit.png"))
    {
        return EXIT_FAILURE;
    }

    levelExit.sprite.setTexture(levelExitTexture);

    //explostion texture
    if(!explosionTexture.loadFromFile("files\\sprites\\explosion.png"))
    {
        return EXIT_FAILURE;
    }

    for(int i = 0; i < NUMBER_OF_EXPLOSIONS; i++)
    {
        explosion.at(i).sprite.setTexture(explosionTexture);
    }

    //collection texture
    if(!collectionTexture.loadFromFile("files\\sprites\\collection.png"))
    {
        return EXIT_FAILURE;
    }

    for(int i = 0; i < NUMBER_OF_COLLECTION_ANIMATIONS; i++)
    {
        collection.at(i).sprite.setTexture(collectionTexture);
    }

    //main sprite
    if(!playerTexture.loadFromFile("files\\sprites\\player.png"))
    {
        return EXIT_FAILURE;
    }

    playerSprite.setTexture(playerTexture);

    //load frames for main sprite
    for(int i = 0; i < NUMBER_OF_PLAYER_SPRITE_FRAMES; i++)
    {
        playerFrames[i] = sf::IntRect(i * PLAYER_WIDTH, 0, PLAYER_WIDTH, PLAYER_HEIGHT);
    }

    playerSprite.setTextureRect(playerFrames[playerFrameNumber]);

    //horPlatform
    if(!platformTexture.loadFromFile("files\\sprites\\platform.png"))
    {
        return EXIT_FAILURE;
    }

    horPlatformSprite.setTexture(platformTexture);
    vertPlatformSprite.setTexture(platformTexture);

    //ending graphics
    if(!endingHumanTexture.loadFromFile("files\\sprites\\human.png"))
    {
        return EXIT_FAILURE;
    }

    endingHumanSprite.setTexture(endingHumanTexture);

    endingHumanSprite.setColor(sf::Color(255, 255, 255, endingHumanOpacity));

    endingHumanSprite.setPosition(SCREEN_WIDTH_PIXELS / 2, SCREEN_HEIGHT_PIXELS / 2);

    if(!endingRobotTexture.loadFromFile("files\\sprites\\robot.png"))
    {
        return EXIT_FAILURE;
    }

    endingRobotSprite.setTexture(endingRobotTexture);

    endingRobotSprite.setColor(sf::Color(255, 255, 255, endingRobotOpacity));

    endingRobotSprite.setPosition(SCREEN_WIDTH_PIXELS / 2, SCREEN_HEIGHT_PIXELS / 2);

    //prepeare level
    for(int i = 0; i < NUMBER_OF_TILE_IMAGES; i++)
    {
        if(!tileImages[i].loadFromFile("files\\tiles\\tile" + toString(i) + ".png"))
        {
            return EXIT_FAILURE;
        }

    }

    //collision points
    point1.x = 0.0;
    point1.y = 0.0;
    point2.x = 0.0;
    point2.y = 0.0;
    point3.x = 0.0;
    point3.y = 0.0;
    point4.x = 0.0;
    point4.y = 0.0;
    point5.x = 0.0;
    point5.y = 0.0;
    point6.x = 0.0;
    point6.y = 0.0;
    point7.x = 0.0;
    point7.y = 0.0;
    point8.x = 0.0;
    point8.y = 0.0;
    point5A.x = 0.0;
    point5A.y = 0.0;
    point6A.x = 0.0;
    point6A.y = 0.0;

    //load sfx
    jumpSoundBuffer.loadFromFile("files\\sounds\\jump.wav");
    jumpSound.setBuffer(jumpSoundBuffer);

    headhitSoundBuffer.loadFromFile("files\\sounds\\headhit.wav");
    headhitSound.setBuffer(headhitSoundBuffer);

    hitenemySoundBuffer.loadFromFile("files\\sounds\\hitenemy.wav");
    hitenemySound.setBuffer(hitenemySoundBuffer);

    keytypeSoundBuffer.loadFromFile("files\\sounds\\keytype.wav");
    keytypeSound.setBuffer(keytypeSoundBuffer);

    pickupSoundBuffer.loadFromFile("files\\sounds\\pickup.wav");
    pickupSound.setBuffer(pickupSoundBuffer);

    hurtSoundBuffer.loadFromFile("files\\sounds\\hurt.wav");
    hurtSound.setBuffer(hurtSoundBuffer);

    footstepSoundBuffer.loadFromFile("files\\sounds\\footstep.wav");
    footstepSound.setBuffer(footstepSoundBuffer);

    firstwarningSoundBuffer.loadFromFile("files\\sounds\\firstwarning.wav");
    firstwarningSound.setBuffer(firstwarningSoundBuffer);

    secondwarningSoundBuffer.loadFromFile("files\\sounds\\secondwarning.wav");
    secondwarningSound.setBuffer(secondwarningSoundBuffer);

    thirdwarningSoundBuffer.loadFromFile("files\\sounds\\thirdwarning.wav");
    thirdwarningSound.setBuffer(thirdwarningSoundBuffer);

    fourthwarningSoundBuffer.loadFromFile("files\\sounds\\fourthwarning.wav");
    fourthwarningSound.setBuffer(fourthwarningSoundBuffer);

    exitSoundBuffer.loadFromFile("files\\sounds\\exit.wav");
    exitSound.setBuffer(exitSoundBuffer);

    for(int i = 0; i < NUMBER_OF_TILES; i++)
    {
        blocks[i].x = 0.0;
        blocks[i].y = 0.0;
    }

    //level list
    ifstream levelListFile;
    levelListFile.open("files\\levels.txt");
    string levelNames;

    //text
    if(!atariFont.loadFromFile("files\\atariFont.ttf"))
    {
        return EXIT_FAILURE;
    }

    pressEnterText.setString("Press Enter to begin");
    pressEnterText.setFont(atariFont);
    pressEnterText.setCharacterSize(10);
    pressEnterText.setColor(sf::Color::Black);
    pressEnterText.setPosition(580.0, 580.0);

    pressFText.setString("Press F to toggle FullScreen");
    pressFText.setFont(atariFont);
    pressFText.setCharacterSize(10);
    pressFText.setColor(sf::Color::Black);
    pressFText.setPosition(10.0, 10.0);

    pressEscapeText.setString("Press ESC for options");
    pressEscapeText.setFont(atariFont);
    pressEscapeText.setCharacterSize(10);
    pressEscapeText.setColor(sf::Color::Black);
    pressEscapeText.setPosition(20.0, 10.0);

    countDownTimerText.setString(toString(COUNTDOWN_TIMER_RESET));
    countDownTimerText.setFont(atariFont);
    countDownTimerText.setCharacterSize(30);
    countDownTimerText.setColor(sf::Color::White);
    countDownTimerText.setPosition(700.0, 20.0);

    gmenu_gamePausedText.setString("GAME PAUSED");
    gmenu_gamePausedText.setFont(atariFont);
    gmenu_gamePausedText.setCharacterSize(20);
    gmenu_gamePausedText.setColor(sf::Color::White);
    gmenu_gamePausedText.setPosition(270.0, 160.0);

    gmenu_pressEscText.setString("Press Esc to resume");
    gmenu_pressEscText.setFont(atariFont);
    gmenu_pressEscText.setCharacterSize(10);
    gmenu_pressEscText.setColor(sf::Color::White);
    gmenu_pressEscText.setPosition(275.0, 190.0);

    gmenu_qForTitleText.setString("Press Q to Quit");
    gmenu_qForTitleText.setFont(atariFont);
    gmenu_qForTitleText.setCharacterSize(10);
    gmenu_qForTitleText.setColor(sf::Color::White);
    gmenu_qForTitleText.setPosition(210.0, 240.0);

    gmenu_fToggleFScreen.setString("Press F to toggle fullscreen");
    gmenu_fToggleFScreen.setFont(atariFont);
    gmenu_fToggleFScreen.setCharacterSize(10);
    gmenu_fToggleFScreen.setColor(sf::Color::White);
    gmenu_fToggleFScreen.setPosition(210.0, 260.0);

    gmenu_vToggleVsync.setString("Press V to toggle VSync");
    gmenu_vToggleVsync.setFont(atariFont);
    gmenu_vToggleVsync.setCharacterSize(10);
    gmenu_vToggleVsync.setColor(sf::Color::White);
    gmenu_vToggleVsync.setPosition(210.0, 280.0);

    gmenu_sChangeFPS.setString("Press S to set FPS limit");
    gmenu_sChangeFPS.setFont(atariFont);
    gmenu_sChangeFPS.setCharacterSize(10);
    gmenu_sChangeFPS.setColor(sf::Color::White);
    gmenu_sChangeFPS.setPosition(210.0, 300.0);

    gmenu_fullscreendisplay.setString("Full Screen: OFF");
    gmenu_fullscreendisplay.setFont(atariFont);
    gmenu_fullscreendisplay.setCharacterSize(10);
    gmenu_fullscreendisplay.setColor(sf::Color::White);
    gmenu_fullscreendisplay.setPosition(210.0, 340.0);

    gmenu_vsyncdisplay.setString("VSync: OFF");
    gmenu_vsyncdisplay.setFont(atariFont);
    gmenu_vsyncdisplay.setCharacterSize(10);
    gmenu_vsyncdisplay.setColor(sf::Color::White);
    gmenu_vsyncdisplay.setPosition(210.0, 360.0);

    gmenu_fpsdisplay.setString("FPS limit: 70");
    gmenu_fpsdisplay.setFont(atariFont);
    gmenu_fpsdisplay.setCharacterSize(10);
    gmenu_fpsdisplay.setColor(sf::Color::White);
    gmenu_fpsdisplay.setPosition(210.0, 380.0);

    tmenu_menuText.setString("MENU");
    tmenu_menuText.setFont(atariFont);
    tmenu_menuText.setCharacterSize(20);
    tmenu_menuText.setColor(sf::Color::White);
    tmenu_menuText.setPosition(350.0, 160.0);

    tmenu_pressEscText.setString("Press Esc to leave menu");
    tmenu_pressEscText.setFont(atariFont);
    tmenu_pressEscText.setCharacterSize(10);
    tmenu_pressEscText.setColor(sf::Color::White);
    tmenu_pressEscText.setPosition(275.0, 190.0);

    tmenu_qForOSText.setString("Press Q to return to Windows");
    tmenu_qForOSText.setFont(atariFont);
    tmenu_qForOSText.setCharacterSize(10);
    tmenu_qForOSText.setColor(sf::Color::White);
    tmenu_qForOSText.setPosition(210.0, 240.0);

    countDownTimerBg.setSize(sf::Vector2f(100.0, 50.0));
    countDownTimerBg.setPosition(690, 10);
    countDownTimerBg.setFillColor(sf::Color::Black);
    //countDownTimerBg.setOutlineThickness(5);
    //countDownTimerBg.setOutlineColor(sf::Color::Blue);

    diagonaluniverseText.setString("www.diagonaluniverse.com");
    diagonaluniverseText.setFont(atariFont);
    diagonaluniverseText.setCharacterSize(10);
    diagonaluniverseText.setColor(sf::Color::Red);
    diagonaluniverseText.setPosition(10.0, 580.0);

    while(!levelListFile.eof())
    {
        getline(levelListFile, levelNames, ',');
        levelListVector.push_back("files\\levels\\" + levelNames + ".txt");
    }

    levelListFile.close();

    //load the first level
    LoadLevel("files\\levels\\titlescr.txt");

    //load music files
    if (!titleScrMusic.openFromFile("files\\sounds\\titleScrMusic.ogg"))
    {
        return EXIT_FAILURE;
    }

    if (!inGameMusic.openFromFile("files\\sounds\\inGameMusic.ogg"))
    {
        return EXIT_FAILURE;
    }

    //music.play();
    //music.setLoop(5);
    //music.setPitch(1.0);
    inGameMusic.setVolume(0.75);

    titleScrMusic.play();

    titleScrMusic.setLoop(100);
    inGameMusic.setLoop(100);

    //seed for random numbers
    srand(12632);

    //lines for title screen
    for(int i = 0; i < NUMBER_OF_LINES; i++)
    {
        lines[i].sprite.setSize(sf::Vector2f(LINE_WIDTH, LINE_HEIGHT));
        lines[i].sprite.setFillColor(LINE_COLOR);
    }

    //for in game menu and title screen menu
    menuBox.setSize(sf::Vector2f(400.0, 300.0));
    menuBox.setFillColor(sf::Color::Black);
    menuBox.setPosition(200.0, 150.0);

    while (game1.isOpen())
    {
        while (game1.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                game1.close();
            }
        }

        loops = 0;
        while(aClock.getElapsedTime().asMilliseconds() > next_game_tick && loops < MAX_FRAMESKIP)
        {
            //update game states
            if(gameState == gamePlay)
            {
                updateGameplay();
            }

            if(gameState == titleScreen)
            {
                updateTitleScreen();
            }

            if(gameState == intro)
            {
                updateIntro();
            }

            if(gameState == goodEnding)
            {
                updateGoodEnding();
            }

            if(gameState == badEnding)
            {
                updateBadEnding();
            }

            next_game_tick += SKIP_TICKS;
            loops++;
        }

        interpolation = float(aClock.getElapsedTime().asMilliseconds() + SKIP_TICKS - next_game_tick) / float(SKIP_TICKS );

        if(gameState == gamePlay)
        {
            drawGameplay();
        }

        if(gameState == titleScreen)
        {
            drawTitleScreen();
        }

        if(gameState == intro)
        {
            drawIntro();
        }

        if(gameState == goodEnding)
        {
            drawGoodEnding();
        }

        if(gameState == badEnding)
        {
            drawBadEnding();
        }
    }

    return 0;
}

void updateTitleScreen()
{
    //handle lines effect on title screen
    for(int i = 0; i < NUMBER_OF_LINES; i++)
    {
        if(!lines[i].active)
        {
            lines[i].active = true;
            lines[i].yPos = i * 20.0;
        }

        if(lines[i].yPos > 520.0)
        {
            lines[i].yPos = 40.0f;
        }

        lines[i].yPos += LINE_SPEED;
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Return) && returnPressed == false && !showTitleScreenMenu)
    {
        returnPressed = true;
        gameState = intro;
    }

    if(!sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
    {
        returnPressed = false;
    }

    if(!sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
    {
        qPressed = false;
    }

    //deal with title screen options menu
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) && escapePressed == false)
    {
        escapePressed = true;
        showTitleScreenMenu = !showTitleScreenMenu;

    }

    if(!sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
    {
        escapePressed = false;
    }

    if(showTitleScreenMenu)
    {
        toggleFullScreen();
        toggleVSync();
        toggleFPS();

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q) && qPressed == false)
        {
            game1.close();
        }

        if(!sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
        {
            qPressed = false;
        }
    }
}

void updateGameplay()
{
    ////////////////points//////////////////////
    if(layer2[(int)point1.x / TILE_WIDTH][(int)point1.y / TILE_HEIGHT] > 0)
    {
        point1Collision = true;
    }

    if(layer2[(int)point1.x / TILE_WIDTH][(int)point1.y / TILE_HEIGHT] <= 0)
    {
        point1Collision = false;
    }

    if(layer2[(int)point2.x / TILE_WIDTH][(int)point2.y / TILE_HEIGHT] > 0)
    {
        point2Collision = true;
    }

    if(layer2[(int)point2.x / TILE_WIDTH][(int)point2.y / TILE_HEIGHT] <= 0)
    {
        point2Collision = false;
    }

    if(layer2[(int)point3.x / TILE_WIDTH][(int)point3.y / TILE_HEIGHT] > 0)
    {
        point3Collision = true;
    }

    if(layer2[(int)point3.x / TILE_WIDTH][(int)point3.y / TILE_HEIGHT] <= 0)
    {
        point3Collision = false;
    }

    if(layer2[(int)point4.x / TILE_WIDTH][(int)point4.y / TILE_HEIGHT] > 0)
    {
        point4Collision = true;
    }

    if(layer2[(int)point4.x / TILE_WIDTH][(int)point4.y / TILE_HEIGHT] <= 0)
    {
        point4Collision = false;
    }

    if(layer2[(int)point5.x / TILE_WIDTH][(int)point5.y / TILE_HEIGHT] > 0)
    {
        point5Collision = true;
    }

    if(layer2[(int)point5.x / TILE_WIDTH][(int)point5.y / TILE_HEIGHT] <= 0)
    {
        point5Collision = false;
    }

    if(layer2[(int)point6.x / TILE_WIDTH][(int)point6.y / TILE_HEIGHT] > 0)
    {
        point6Collision = true;
    }

    if(layer2[(int)point6.x / TILE_WIDTH][(int)point6.y / TILE_HEIGHT] <= 0)
    {
        point6Collision = false;
    }

    if(layer2[(int)point7.x / TILE_WIDTH][(int)point7.y / TILE_HEIGHT] > 0)
    {
        point7Collision = true;
    }

    if(layer2[(int)point7.x / TILE_WIDTH][(int)point7.y / TILE_HEIGHT] <= 0)
    {
        point7Collision = false;
    }

    if(layer2[(int)point8.x / TILE_WIDTH][(int)point8.y / TILE_HEIGHT] > 0)
    {
        point8Collision = true;
    }

    if(layer2[(int)point8.x / TILE_WIDTH][(int)point8.y / TILE_HEIGHT] <= 0)
    {
        point8Collision = false;
    }

    if(layer2[(int)point5A.x / TILE_WIDTH][(int)point5A.y / TILE_HEIGHT] > 0)
    {
        point5ACollision = true;
    }

    if(layer2[(int)point5A.x / TILE_WIDTH][(int)point5A.y / TILE_HEIGHT] <= 0)
    {
        point5ACollision = false;
    }

    if(layer2[(int)point6A.x / TILE_WIDTH][(int)point6A.y / TILE_HEIGHT] > 0)
    {
        point6ACollision = true;
    }

    if(layer2[(int)point6A.x / TILE_WIDTH][(int)point6A.y / TILE_HEIGHT] <= 0)
    {
        point6ACollision = false;
    }

    //z to jump
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Z) && jumping == false && playerHurt == 0)
    {
        jumping = true;

        extraJumpHeight = playerSpeedX / 2.0;

        //make extra jump height a positive if it happens to be a negative number
        if(extraJumpHeight < 0.0)
        {
            extraJumpHeight = extraJumpHeight * - 1.0;
        }

        //cap the jumpheight
        if(extraJumpHeight > 4.0)
        {
            extraJumpHeight = 4.0;
        }

        playerSpeedY = JUMP_HEIGHT - extraJumpHeight;
        jumpSound.play();
    }

    if(!sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && playerHurt == 0)
    {
        if(playerSpeedX > 0.0)
        {
            playerSpeedX -= SLOW_DOWN_AMOUNT * over_all_game_speed;

            if(playerSpeedX < 0.0)
            {
                playerSpeedX = 0.0;
            }
        }


        if(playerSpeedX < 0.0)
        {
            playerSpeedX += SLOW_DOWN_AMOUNT * over_all_game_speed;

            if(playerSpeedX > 0.0)
            {
                playerSpeedX = 0.0;
            }
        }
    }

    //code to make the player walk
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && playerHurt == 0)
    {
        facingRight = false;

        if(playerSpeedX > -MAX_X_SPEED)
        {
            if(!jumping)
            {
                playerSpeedX -= SPEED_UP_AMOUNT * over_all_game_speed;
            }

            if(jumping)
            {
                playerSpeedX -= (SPEED_UP_AMOUNT / 2.0) * over_all_game_speed;
            }
        }

        if(playerSpeedX < -MAX_X_SPEED)
        {
            playerSpeedX = -MAX_X_SPEED;
         }
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && playerHurt == 0)
    {
        facingRight = true;

        if(playerSpeedX < MAX_X_SPEED)
        {
            if(!jumping)
            {
                playerSpeedX += SPEED_UP_AMOUNT * over_all_game_speed;
            }

            if(jumping)
            {
                playerSpeedX += (SPEED_UP_AMOUNT / 2.0) * over_all_game_speed;
            }
        }

        if(playerSpeedX > MAX_X_SPEED)
        {
            playerSpeedX = MAX_X_SPEED;
        }
    }

    if(playerSpeedX > 0.0)
    {
        if(!point6Collision && !point8Collision && !point6ACollision)
	    {
	        maxMoveRight = (TILES_ACROSS * TILE_WIDTH) - PLAYER_WIDTH;
	        playerX += playerSpeedX * over_all_game_speed;
	    }

        if(point6Collision || point8Collision || point6ACollision || point6.x > (TILES_ACROSS * TILE_WIDTH))
        {
	        maxMoveRight = (int)point6.x;

            while(maxMoveRight % TILE_WIDTH != 0)
            {
                maxMoveRight--;
            }

            maxMoveRight = maxMoveRight - PLAYER_WIDTH;

            if((playerX + (playerSpeedX)) < maxMoveRight)
            {
                playerX += playerSpeedX * over_all_game_speed;
            }

		    if((playerX + (playerSpeedX)) >= maxMoveRight)
            {
                playerX = maxMoveRight;
                playerSpeedX = 0.0;
            }
	    }
    }

    if(playerSpeedX < 0.0)
    {
        if(!point5Collision && !point7Collision && !point5ACollision)
	    {
	        maxMoveLeft = 0;
	        playerX += playerSpeedX * over_all_game_speed;
	    }

        if(point5Collision || point7Collision || point5ACollision || point5.x < 0.0)
        {
	        maxMoveLeft = (int)point5.x;

            while(maxMoveLeft % TILE_WIDTH != 0)
            {
                maxMoveLeft++;
            }

            if((playerX + (playerSpeedX)) > maxMoveLeft)
            {
                playerX += playerSpeedX * over_all_game_speed;
            }

		    if((playerX + (playerSpeedX)) <= maxMoveLeft)
            {
                playerX = maxMoveLeft;
                playerSpeedX = 0.0;
            }
	    }
    }

    if(!onHorPlatform && !onVertPlatform)
    {
        if(!point3Collision && !point4Collision && jumping == false)
        {
            jumping = true;
            //playerSpeedY = 0.0;
        }

        if(playerSpeedY > 0.0)
        {
            if(!point3Collision && !point4Collision)
	        {
	            maxMoveDown = TILES_DOWN * TILE_HEIGHT;
	            playerY += playerSpeedY * over_all_game_speed;
	        }

            if(point3Collision || point4Collision)
            {
	            maxMoveDown = (int)point3.y;

                while(maxMoveDown % TILE_HEIGHT != 0)
                {
                    maxMoveDown--;
                }

		        maxMoveDown = maxMoveDown - PLAYER_HEIGHT;

                if((playerY + (playerSpeedY)) < maxMoveDown)
                {
                    playerY += playerSpeedY * over_all_game_speed;
                }

		        if((playerY + (playerSpeedY)) >= maxMoveDown)
                {
                    playerY = maxMoveDown;
                    jumping = false;
                    playerSpeedY = 0.0;
                }
	        }
        }
    }

    //jumping
    if(jumping == true && playerSpeedY < MAX_Y_SPEED)
    {
        playerSpeedY += GRAVITY;

        if(playerSpeedY > MAX_Y_SPEED)
        {
            playerSpeedY = MAX_Y_SPEED;
        }
    }

    if(playerSpeedY < 0.0)
    {
        if(!point1Collision && !point2Collision)
	    {
	        maxMoveUp = 0;
	        playerY += playerSpeedY * over_all_game_speed;
	    }

        if(point1Collision || point2Collision || point1.y < 0.0)
        {
	        maxMoveUp = (int)point1.y;

	        while(maxMoveUp % TILE_HEIGHT != 0)
            {
                maxMoveUp++;
            }

            if((playerY + (playerSpeedY)) > maxMoveUp)
            {
                playerY += playerSpeedY * over_all_game_speed;
            }

		    if((playerY + (playerSpeedY)) <= maxMoveUp)
            {
                playerY = maxMoveUp;
                playerSpeedY = 0.0;

                if(canPlayHeadHitSound)
	            {
	                headhitSound.play();
	                canPlayHeadHitSound = false;
	            }
            }
	    }
    }

    //make sure head hit sound only plays once
    if(!point1Collision && !point2Collision && point1.y >= 0.0)
    {
        canPlayHeadHitSound = true;
    }

    //animate the main sprite
    if(facingRight && playerSpeedX != 0.0 && !jumping) //walking right
    {
        if(playerFrameNumber > 1)
        {
            playerFrameNumber = 0;
            animationStep = 0.0;
        }

        if(animationStep >= 60.0)
        {
            playerFrameNumber++;
            animationStep = 0.0;

            if(playFootSound)
            {
                footstepSound.play();
            }

            playFootSound = !playFootSound;
        }

        animationStep += PLAYER_ANIMATION_SPEED * over_all_game_speed;

        if(playerFrameNumber > 1)
        {
            playerFrameNumber = 0;
        }
    }

    if(facingRight && playerSpeedX == 0.0 && !jumping) //standing facing right
    {
        playerFrameNumber = 0;

        //animationStep += PLAYER_ANIMATION_SPEED;

        //if(animationStep >= 1820.0)
        //{
        //    playerFrameNumber = 1;
        //}

        //if(animationStep >= 1920.0)
        //{
        //    playerFrameNumber = 0;
        //    animationStep = 0;
        //}

    }

    if(facingRight && jumping) //jumping right
    {
        playerFrameNumber = 2;
    }

    if(!facingRight && playerSpeedX != 0.0 && !jumping) //walking left
    {
        if(playerFrameNumber < 4)
        {
            playerFrameNumber = 4;
            animationStep = 0.0;
        }

        if(animationStep >= 60.0)
        {
            playerFrameNumber++;
            animationStep = 0.0;

            if(playFootSound)
            {
                footstepSound.play();
            }

            playFootSound = !playFootSound;
        }

        animationStep += PLAYER_ANIMATION_SPEED;

        if(playerFrameNumber > 5)
        {
            playerFrameNumber = 4;
        }
    }

    if(!facingRight && playerSpeedX == 0.0 && !jumping) //standing facing left
    {
        playerFrameNumber = 5;

        //animationStep += PLAYER_ANIMATION_SPEED;

        //if(animationStep >= 1820.0)
        //{
        //    playerFrameNumber = 5;
        //}

        //if(animationStep >= 1920.0)
        //{
        //    playerFrameNumber = 4;
        //    animationStep = 0;
        //}
    }

    if(!facingRight && jumping) //jumping left
    {
        playerFrameNumber = 3;
    }

    //points
    point1.x = playerX;
    point1.y = playerY - 20;
    point2.x = playerX + PLAYER_WIDTH - 1;
    point2.y = playerY - 20;
    point3.x = playerX;
    point3.y = playerY + 20 + PLAYER_HEIGHT;
    point4.x = playerX + PLAYER_WIDTH - 1;
    point4.y = playerY + 20 + PLAYER_HEIGHT;
    point5.x = playerX - 20;
    point5.y = playerY;
    point6.x = playerX + PLAYER_WIDTH + 20;
    point6.y = playerY;
    point7.x = playerX - 20;
    point7.y = playerY + PLAYER_HEIGHT - 1;
    point8.x = playerX + PLAYER_WIDTH + 20;
    point8.y = playerY + PLAYER_HEIGHT - 1;
    point5A.x = playerX - 20;
    point5A.y = playerY + 30;
    point6A.x = playerX + PLAYER_WIDTH + 20;
    point6A.y = playerY + 30;

    //horPlatform
    horPlatformX += horPlatformSpeed;

    if(horPlatformX >= 600.0 || horPlatformX <= 0.0 ||
        layer2[(int)((horPlatformX - 20.0) + horPlatformSpeed) / TILE_WIDTH][(int)horPlatformY / TILE_HEIGHT] > 0 ||
            layer2[(int)(horPlatformX + horPlatformSpeed + 40.0 + 20.0) / TILE_WIDTH][(int)horPlatformY / TILE_HEIGHT] > 0)
    {
        horPlatformSpeed = horPlatformSpeed * -1.0;
    }

    if((vectorInSprite(point3, horPlatformSprite) && playerSpeedY > 0.0) || (vectorInSprite(point4, horPlatformSprite) && playerSpeedY > 0.0))
    {
        onHorPlatform = true;
        jumping = false;
        playerSpeedY = 0.0;
        playerY = horPlatformY - PLAYER_HEIGHT;
    }

    if(onHorPlatform)
    {
        if(!vectorInSprite(point3, horPlatformSprite) && !vectorInSprite(point4, horPlatformSprite))
        {
            onHorPlatform = false;
        }

        if(playerSpeedX == 0.0)
        {
            playerX += horPlatformSpeed;
        }
    }

    //vertPlatform
    vertPlatformY += vertPlatformSpeed;

    if(vertPlatformY >= 470.0 || vertPlatformY <= 40.0 ||
        layer2[(int)vertPlatformX / TILE_WIDTH][(int)(vertPlatformY - 20.0 + vertPlatformSpeed) / TILE_HEIGHT] > 0 ||
            layer2[(int)vertPlatformX / TILE_WIDTH][(int)(vertPlatformY + 10.0 + vertPlatformSpeed) / TILE_HEIGHT] > 0)
    {
        vertPlatformSpeed = vertPlatformSpeed * -1.0;
    }

    if((vectorInSprite(point3, vertPlatformSprite) && playerSpeedY > 0.0) || (vectorInSprite(point4, vertPlatformSprite) && playerSpeedY > 0.0))
    {
        onVertPlatform = true;
        jumping = false;
        playerSpeedY = 0.0;
        playerY = vertPlatformY - 20.0;
    }

    if(onVertPlatform)
    {
        if(!vectorInSprite(sf::Vector2f(point3.x, point3.y - 4), vertPlatformSprite) && !vectorInSprite(sf::Vector2f(point4.x, point4.y - 4), vertPlatformSprite))
        {
            onVertPlatform = false;
        }

        playerY += vertPlatformSpeed;

    }

    //press q w e keys to change background color
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
    {
        color1++;

        if(color1 > 255)
        {
            color1 = 0;
        }
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        color2++;

        if(color2 > 255)
        {
            color2 = 0;
        }
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::E))
    {
        color3++;

        if(color3 > 255)
        {
            color3 = 0;
        }
    }

    //tab to skip levels (debug)
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Tab) && tabPressed == false)
    {
        tabPressed = true;
        levelNumber++;
        LoadLevel(levelListVector.at(levelNumber));
    }

    if(!sf::Keyboard::isKeyPressed(sf::Keyboard::Tab))
    {
        tabPressed = false;
    }

    //update enemy1s
    enemy1AnimationStep += ENEMY1_ANIMATION_SPEED * over_all_game_speed;

    if(enemy1AnimationStep < 100.0)
    {
        enemy1FrameNumber = 0;
    }

    if(enemy1AnimationStep > 100.0 && enemy1AnimationStep < 200.0)
    {
        enemy1FrameNumber = 1;
    }

    if(enemy1AnimationStep > 200.0 && enemy1AnimationStep < 300.0)
    {
        enemy1FrameNumber = 2;
    }

    if(enemy1AnimationStep > 300.0 && enemy1AnimationStep < 400.0)
    {
        enemy1FrameNumber = 3;
    }

    if(enemy1AnimationStep > 400.0 && enemy1AnimationStep < 500.0)
    {
        enemy1FrameNumber = 2;
    }

    if(enemy1AnimationStep > 500.0 && enemy1AnimationStep < 600.0)
    {
        enemy1FrameNumber = 1;
    }

    if(enemy1AnimationStep > 700.0)
    {
        enemy1FrameNumber = 0;
        enemy1AnimationStep = 0.0;
    }

    for(int i = 0; i < NUMBER_OF_EACH_ENEMY; i++)
    {
        enemy1s.at(i).animate(enemy1FrameNumber);

        if(enemy1s.at(i).goingLeft == false)
        {
            enemy1s.at(i).xPos += enemy1s.at(i).speed  * over_all_game_speed;
        }

        if(enemy1s.at(i).goingLeft == true)
        {
            enemy1s.at(i).xPos -= enemy1s.at(i).speed  * over_all_game_speed;
        }

        //enemy1 collides with background
        if(layer2[(int)(enemy1s.at(i).xPos) / TILE_WIDTH][(int)(enemy1s.at(i).yPos + 20.0) / TILE_HEIGHT] > 0
            || layer4[(int)(enemy1s.at(i).xPos) / TILE_WIDTH][(int)(enemy1s.at(i).yPos + 20.0) / TILE_HEIGHT] == 37)
        {
            enemy1s.at(i).goingLeft = false;
        }

        if(layer2[(int)(enemy1s.at(i).xPos + enemy1s.at(i).width) / TILE_WIDTH][(int)(enemy1s.at(i).yPos + 20.0) / TILE_HEIGHT] > 0
            || layer4[(int)(enemy1s.at(i).xPos + enemy1s.at(i).width) / TILE_WIDTH][(int)(enemy1s.at(i).yPos + 20.0) / TILE_HEIGHT] == 37)
        {
            enemy1s.at(i).goingLeft = true;
        }

        //hurt player
        if(Collision(playerSprite, enemy1s.at(i).sprite) && playerSpeedY <= 0.0 && invincibleTime == 0 && playerY > enemy1s.at(i).yPos - PLAYER_HEIGHT + 5)
        {
            playerHurt = 60;
            invincibleTime = 120;
            hurtSound.play();
            countDownTimer -= 10;
        }

        if(Collision(playerSprite, enemy1s.at(i).sprite) && playerSpeedY > 0.0 && playerHurt == 0)
        {
            //animate an explosion where the enemy died
            whichExplosionToDisplay++;

            if(whichExplosionToDisplay > NUMBER_OF_EXPLOSIONS - 1)
            {
                whichExplosionToDisplay = 0;
            }

            explosion.at(whichExplosionToDisplay).alive = true;
            explosion.at(whichExplosionToDisplay).xPos = enemy1s.at(i).xPos;
            explosion.at(whichExplosionToDisplay).yPos = enemy1s.at(i).yPos;

            enemy1s.at(i).kill();
            playerSpeedY = JUMP_HEIGHT;
            hitenemySound.play();
            countDownTimer += 2;
        }

        //hurt player if step on mine
        if(layer2[(int)point3.x / TILE_WIDTH][(int)point3.y / TILE_HEIGHT] == 33
            || layer2[(int)point4.x / TILE_WIDTH][(int)point4.y / TILE_HEIGHT] == 33)
        {
            countDownTimer = -30;
        }
    }

    explosion.at(whichExplosionToDisplay).sprite.setPosition(explosion.at(whichExplosionToDisplay).xPos - offsetX, explosion.at(whichExplosionToDisplay).yPos - offsetY);

    //animate explosions
    for(int i = 0; i < NUMBER_OF_EXPLOSIONS; i++)
    {
        if(explosion.at(i).alive)
        {

            explosion.at(i).animationStep += EXPLOSION_ANIMATION_SPEED;

            if(explosion.at(i).animationStep < 50.0)
            {
                explosion.at(i).frameNumber = 0;
            }

            if(explosion.at(i).animationStep > 50.0 && explosion.at(i).animationStep < 100.0)
            {
                explosion.at(i).frameNumber = 1;
            }

            if(explosion.at(i).animationStep > 100.0 && explosion.at(i).animationStep < 150.0)
            {
                explosion.at(i).frameNumber = 2;
            }

            if(explosion.at(i).animationStep > 150.0 && explosion.at(i).animationStep < 200.0)
            {
                explosion.at(i).frameNumber = 3;
            }

            if(explosion.at(i).animationStep > 200.0 && explosion.at(i).animationStep < 250.0)
            {
                explosion.at(i).frameNumber = 4;
            }

            if(explosion.at(i).animationStep > 250.0 && explosion.at(i).animationStep < 300.0)
            {
                explosion.at(i).frameNumber = 5;
            }

            if(explosion.at(i).animationStep > 300.0 && explosion.at(i).animationStep < 350.0)
            {
                explosion.at(i).frameNumber = 6;
            }

            if(explosion.at(i).animationStep > 350.0 && explosion.at(i).animationStep < 400.0)
            {
                explosion.at(i).frameNumber = 7;
            }

            if(explosion.at(i).animationStep > 450.0)
            {
                explosion.at(i).kill();
            }

            explosion.at(i).animate(explosion.at(i).frameNumber);

        }
    }

    //deal with player being hurt
    if(playerHurt > 0)
    {
        if(over_all_game_speed != 0.0)
        {
            over_all_game_speed = 0.0;
        }

        playerHurt--;
    }

    if(playerHurt == 0)
    {
        if(over_all_game_speed != 1.0)
        {
            over_all_game_speed = 1.0;
        }
    }

    if(invincibleTime > 0)
    {
        invincibleTime--;
    }

    //update pickups
    pickupAnimationStep += PICKUP_ANIMATION_SPEED;

    if(pickupAnimationStep < 100.0)
    {
        pickupFrameNumber = 0;
    }

    if(pickupAnimationStep > 100.0 && pickupAnimationStep < 200.0)
    {
        pickupFrameNumber = 1;
    }

    if(pickupAnimationStep > 200.0 && pickupAnimationStep < 300.0)
    {
        pickupFrameNumber = 2;
    }

    if(pickupAnimationStep > 300.0 && pickupAnimationStep < 400.0)
    {
        pickupFrameNumber = 3;
    }

    if(pickupAnimationStep > 400.0 && pickupAnimationStep < 500.0)
    {
        pickupFrameNumber = 4;
    }

    if(pickupAnimationStep > 500.0 && pickupAnimationStep < 600.0)
    {
        pickupFrameNumber = 5;
    }

	if(pickupAnimationStep > 600.0 && pickupAnimationStep < 700.0)
    {
        pickupFrameNumber = 6;
    }

    if(pickupAnimationStep > 800.0)
    {
        pickupFrameNumber = 0;
        pickupAnimationStep = 0.0;
    }

    for(int i = 0; i < NUMBER_OF_PICKUPS; i++)
    {
        pickups.at(i).animate(pickupFrameNumber);

        if(Collision(playerSprite, pickups.at(i).sprite))
        {
            //animate at the collection point
            whichCollectionToDisplay++;

            if(whichCollectionToDisplay > NUMBER_OF_COLLECTION_ANIMATIONS - 1)
            {
                whichCollectionToDisplay = 0;
            }

            collection.at(whichCollectionToDisplay).alive = true;
            collection.at(whichCollectionToDisplay).xPos = pickups.at(i).xPos;
            collection.at(whichCollectionToDisplay).yPos = pickups.at(i).yPos;

            pickups.at(i).kill();
            countDownTimer += 2;
            pickupSound.play();
        }
    }

    collection.at(whichCollectionToDisplay).sprite.setPosition(collection.at(whichCollectionToDisplay).xPos - offsetX, collection.at(whichCollectionToDisplay).yPos - offsetY);

    //animate Collections
    for(int i = 0; i < NUMBER_OF_COLLECTION_ANIMATIONS; i++)
    {
        if(collection.at(i).alive)
        {

            collection.at(i).animationStep += COLLECTION_ANIMATION_SPEED;

            if(collection.at(i).animationStep < 50.0)
            {
                collection.at(i).frameNumber = 0;
            }

            if(collection.at(i).animationStep > 50.0 && collection.at(i).animationStep < 100.0)
            {
                collection.at(i).frameNumber = 1;
            }

            if(collection.at(i).animationStep > 100.0 && collection.at(i).animationStep < 150.0)
            {
                collection.at(i).frameNumber = 2;
            }

            if(collection.at(i).animationStep > 150.0 && collection.at(i).animationStep < 200.0)
            {
                collection.at(i).frameNumber = 3;
            }

            if(collection.at(i).animationStep > 200.0 && collection.at(i).animationStep < 250.0)
            {
                collection.at(i).frameNumber = 4;
            }

            if(collection.at(i).animationStep > 250.0 && collection.at(i).animationStep < 300.0)
            {
                collection.at(i).frameNumber = 5;
            }

            if(collection.at(i).animationStep > 350.0)
            {
                collection.at(i).kill();
            }

            collection.at(i).animate(collection.at(i).frameNumber);

        }
    }

    //countdown the continuum timer
    secondsCounter++;

    if(secondsCounter == 60)
    {
        secondsCounter = 0;

        countDownTimer--;

        countDownTimerText.setString(toString(countDownTimer));
    }

    if(countDownTimer >= 0)
    {
        if(anomelyIntensity != none)
        {
            anomelyIntensity = none;

            makeAllTilesWhite();
        }
    }

    if(countDownTimer < 0 && countDownTimer > -10)
    {
        if(anomelyIntensity != low)
        {
            anomelyIntensity = low;

            makeAllTilesWhite();
        }
    }

    if(countDownTimer < -10)
    {
        if(anomelyIntensity != medium)
        {
            anomelyIntensity = medium;
        }
    }

    if(countDownTimer < -20)
    {
        if(anomelyIntensity != high)
        {
            anomelyIntensity = high;
        }
    }

    //mess with anomely's
    if(anomelyIntensity == low)
    {
        effectChange++;

        if(effectChange >= 50)
        {
            color1 = rand() % 255;
            color2 = rand() % 255;
            color3 = rand() % 255;

            /*changeTilesToColor = sf::Color(rand() % 255, rand() % 255, rand() % 255);

            for(int i = 0; i < NUMBER_OF_TILES; i++)
            {
                //limit how many tiles are effected to those that are visible
			    if(blocks[i].x > playerX - (SCREEN_WIDTH_PIXELS - 40 / 2)
			        && blocks[i].x < playerX + (SCREEN_WIDTH_PIXELS + 40 / 2)
			            && blocks[i].y > playerY - (SCREEN_HEIGHT_PIXELS - 40 / 2)
			                && blocks[i].y < playerY + (SCREEN_HEIGHT_PIXELS + 40 / 2)
			                    && blocks[i].sprite.getColor() != changeTilesToColor)
			    {
                    blocks[i].sprite.setColor(changeTilesToColor);
                }
            }*/

            effectChange = 0;
        }
    }

    if(anomelyIntensity == medium)
    {
        effectChange++;

        if(effectChange >= 30)
        {
            color1 = rand() % 255;
            color2 = rand() % 255;
            color3 = rand() % 255;

            changeTilesToColor = sf::Color(rand() % 255, rand() % 255, rand() % 255);

            for(int i = 0; i < NUMBER_OF_TILES; i++)
            {
                //limit how many tiles are effected to those that are visible
			    if(blocks[i].x > playerX - (SCREEN_WIDTH_PIXELS - 40 / 2)
			        && blocks[i].x < playerX + (SCREEN_WIDTH_PIXELS + 40 / 2)
			            && blocks[i].y > playerY - (SCREEN_HEIGHT_PIXELS - 40 / 2)
			                && blocks[i].y < playerY + (SCREEN_HEIGHT_PIXELS + 40 / 2)
			                    && blocks[i].sprite.getColor() != changeTilesToColor)
			    {
                    blocks[i].sprite.setColor(changeTilesToColor);
                }
            }

            for(int i = 0; i < NUMBER_OF_EACH_ENEMY; i++)
            {
                if(enemy1s.at(i).speed < 0.0)
                {
                    enemy1s.at(i).speed = -4.0;
                }

                if(enemy1s.at(i).speed >= 0.0)
                {
                    enemy1s.at(i).speed = 4.0;
                }
            }

            effectChange = 0;
        }
    }

    if(anomelyIntensity == high)
    {
        effectChange++;

        if(effectChange >= 10)
        {
            color1 = rand() % 255;
            color2 = rand() % 255;
            color3 = rand() % 255;

            for(int i = 0; i < NUMBER_OF_TILES; i++)
            {
                //limit how many tiles are effected to those that are visible
			    if(blocks[i].x > playerX - (SCREEN_WIDTH_PIXELS - 40 / 2)
			        && blocks[i].x < playerX + (SCREEN_WIDTH_PIXELS + 40 / 2)
			            && blocks[i].y > playerY - (SCREEN_HEIGHT_PIXELS - 40 / 2)
			                && blocks[i].y < playerY + (SCREEN_HEIGHT_PIXELS + 40 / 2))
			    {
                    blocks[i].sprite.setColor(sf::Color(rand() % 255, rand() % 255, rand() % 255));
                }
            }

            for(int i = 0; i < NUMBER_OF_EACH_ENEMY; i++)
            {
                if(enemy1s.at(i).speed < 0.0)
                {
                    enemy1s.at(i).speed = -8.0;
                }

                if(enemy1s.at(i).speed >= 0.0)
                {
                    enemy1s.at(i).speed = 8.0;
                }
            }

            effectChange = 0;
        }
    }

    //reset settings for no anomely
    if(anomelyIntensity == none)
    {
        color1 = 100;
        color2 = 100;
        color3 = 100;

        for(int i = 0; i < NUMBER_OF_TILES; i++)
        {
            //limit how many tiles are effected to those that are visible
			if(blocks[i].x > playerX - (SCREEN_WIDTH_PIXELS - 40 / 2)
			    && blocks[i].x < playerX + (SCREEN_WIDTH_PIXELS + 40 / 2)
			        && blocks[i].y > playerY - (SCREEN_HEIGHT_PIXELS - 40 / 2)
			            && blocks[i].y < playerY + (SCREEN_HEIGHT_PIXELS + 40 / 2)
			                && blocks[i].sprite.getColor() != sf::Color::White)
			{
                blocks[i].sprite.setColor(sf::Color::White);
            }
        }

        for(int i = 0; i < NUMBER_OF_EACH_ENEMY; i++)
        {
            if(enemy1s.at(i).speed < 0.0)
            {
                enemy1s.at(i).speed = -2.0;
            }

            if(enemy1s.at(i).speed >= 0.0)
            {
                enemy1s.at(i).speed = 2.0;
            }
        }

        effectChange = 0;
    }

    //change player color if hurt
    if(playerHurt > 0 && playerSprite.getColor() != sf::Color::Red)
    {
        playerSprite.setColor(sf::Color::Red);
    }

    if(playerHurt == 0 && invincibleTime == 0 && playerSprite.getColor() != sf::Color::White)
    {
        playerSprite.setColor(sf::Color::White);
    }

    if(playerHurt == 0 && invincibleTime > 0)
    {
        playerSprite.setColor(sf::Color::Red);
    }

    //deal with in game menu / pause menu
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) && escapePressed == false)
    {
        escapePressed = true;
        showInGameMenu = !showInGameMenu;

        if(showInGameMenu == false)
        {
            over_all_game_speed = 1.0;
        }
    }

    if(!sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
    {
        escapePressed = false;
    }

    if(showInGameMenu)
    {
        over_all_game_speed = 0.0;
        toggleFullScreen();
        toggleVSync();
        toggleFPS();

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q) && qPressed == false)
        {
            qPressed = true;
            resetVars();
            LoadLevel("files\\levels\\titlescr.txt");
            gameState = titleScreen;

            inGameMusic.stop();
            titleScrMusic.play();
        }

        if(!sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
        {
            qPressed = false;
        }
    }

    //deal with death
    if(countDownTimer <= -30)
    {
        dead = true;

        for(int i = 0; i < NUMBER_OF_EACH_ENEMY; i++)
        {
            if(enemy1s.at(i).alive)
            {
                enemy1s.at(i).kill();
            }
        }

        for(int i = 0; i < NUMBER_OF_PICKUPS; i++)
        {
            if(pickups.at(i).alive)
            {
                pickups.at(i).kill();
            }
        }
    }

    if(countDownTimer > -30)
    {
        dead = false;
    }

    if(dead)
    {
        deadTimer++;

        if(deadTimer == 180)
        {
            deadTimer = 0;
            playerSpeedX = 0.0;
            makeAllTilesWhite();
            LoadLevel(levelListVector.at(levelNumber));
        }
    }

    if(countDownTimer <= 0 && countDownTimer > -10 && !firstwarningPlayed)
    {
        firstwarningPlayed = true;
        firstwarningSound.play();
    }

    if(countDownTimer <= -10 && countDownTimer > -20 && !secondwarningPlayed)
    {
        secondwarningPlayed = true;
        secondwarningSound.play();
    }

    if(countDownTimer <= -20 && countDownTimer > -30 && !thirdwarningPlayed)
    {
        thirdwarningPlayed = true;
        thirdwarningSound.play();
    }

    if(countDownTimer <= -30 && !fourthwarningPlayed)
    {
        fourthwarningPlayed = true;
        fourthwarningSound.play();
    }

    if(countDownTimer > 0)
    {
        firstwarningPlayed = false;
    }

    if(countDownTimer > -10)
    {
        secondwarningPlayed = false;
    }

    if(countDownTimer > -20)
    {
        thirdwarningPlayed = false;
    }

    if(countDownTimer > -30)
    {
        fourthwarningPlayed = false;
    }

    //level exit
    levelExit.animate(pickupFrameNumber);

    if(Collision(playerSprite, levelExit.sprite) && exitingLevel == false)
    {
        exitingLevel = true;

        exitSound.play();

        if(countDownTimer < 10)
        {
            countDownTimer = 10;
        }
    }

    if(exitingLevel == true)
    {
        timeBeforeNextLevelLoads--;
        over_all_game_speed = 0.0;

        if(timeBeforeNextLevelLoads == 0)
        {
            levelNumber++;
            playerSpeedX = 0.0;
            exitingLevel = false;
            timeBeforeNextLevelLoads = 120;
            makeAllTilesWhite();

            if(levelNumber <= LAST_LEVEL_NUMBER)
            {
                LoadLevel(levelListVector.at(levelNumber));
            }

            if(levelNumber > LAST_LEVEL_NUMBER && numberOfEnemysKilledByPlayer == TOTAL_ENEMYS_IN_GAME)
            {
                results.setString(toString(numberOfEnemysKilledByPlayer) + " errors destroyed out of " + toString(TOTAL_ENEMYS_IN_GAME));
                gameState = goodEnding;
            }

            if(levelNumber > LAST_LEVEL_NUMBER && numberOfEnemysKilledByPlayer < TOTAL_ENEMYS_IN_GAME)
            {
                results.setString(toString(numberOfEnemysKilledByPlayer) + " errors destroyed out of " + toString(TOTAL_ENEMYS_IN_GAME));
                gameState = badEnding;
            }
        }
    }
}

void drawGameplay()
{
    //scroll the camera around the player
    offsetX = playerX - 400.0 + (playerSpeedX * interpolation) * over_all_game_speed;

    if(playerY > 600.0)
    {
        offsetY = 600.0;
    }

    if(playerY > 1200.0)
    {
        offsetY = 1200.0;
    }

    //restrict camera movement while the player is near the edges of the playfield
    if(playerY < 600.0) //i changed this from 300 to 600 not sure if it was the right thing to do
    {
        offsetY = 0.0;
    }

    if(playerX < 400.0)
    {
        offsetX = 0.0;
    }

    // Draw the sprite
    playerSprite.setPosition(playerX - offsetX + (playerSpeedX * interpolation) * over_all_game_speed, playerY - offsetY + (playerSpeedY * interpolation) * over_all_game_speed);

    //horPlatform
    horPlatformSprite.setPosition(horPlatformX - offsetX + (horPlatformSpeed * interpolation), horPlatformY - offsetY);

    //vertPlatform
    vertPlatformSprite.setPosition(vertPlatformX, vertPlatformY + (vertPlatformSpeed * interpolation));

    for(int i = 0; i < NUMBER_OF_TILES; i++)
	{
	    blocks[i].sprite.setPosition(blocks[i].x - offsetX, blocks[i].y - offsetY);
	}

    //draw background
    if(!dead && !exitingLevel)
    {
        game1.clear(sf::Color(color1, color2, color3));
    }

    if(dead)
    {
        game1.clear(sf::Color::Red);
    }

    if(exitingLevel)
    {
        game1.clear(sf::Color::Green);
    }

    drawBlocksCounter = 0;

    //draw layer 1
    for(int i = 0; i < TILES_ACROSS; i++)
    {
	    for(int j = 0; j < TILES_DOWN; j++)
		{
		    if(layer1[i][j] > 0 && !dead && !exitingLevel)
			{
			    //limit how many tiles are rendered to what is visible to the player
			    if(blocks[drawBlocksCounter].x > playerX - (SCREEN_WIDTH_PIXELS - 40 / 2)
			        && blocks[drawBlocksCounter].x < playerX + (SCREEN_WIDTH_PIXELS + 40 / 2)
			            && blocks[drawBlocksCounter].y > playerY - (SCREEN_HEIGHT_PIXELS - 40 / 2)
			                && blocks[drawBlocksCounter].y < playerY + (SCREEN_HEIGHT_PIXELS + 40 / 2))

			    {
			        game1.draw(blocks[drawBlocksCounter].sprite);
			    }
			}

			drawBlocksCounter++;
        }
	}

    //draw layer 2
	for(int i = 0; i < TILES_ACROSS; i++)
    {
	    for(int j = 0; j < TILES_DOWN; j++)
		{
		    if(layer2[i][j] > 0 && !dead && !exitingLevel)
			{
			    //limit how many tiles are rendered to what is visible to the player
			    if(blocks[drawBlocksCounter].x > playerX - (SCREEN_WIDTH_PIXELS - 40 / 2)
			        && blocks[drawBlocksCounter].x < playerX + (SCREEN_WIDTH_PIXELS + 40 / 2)
			            && blocks[drawBlocksCounter].y > playerY - (SCREEN_HEIGHT_PIXELS - 40 / 2)
			                && blocks[drawBlocksCounter].y < playerY + (SCREEN_HEIGHT_PIXELS + 40 / 2))

			    {
			        game1.draw(blocks[drawBlocksCounter].sprite);
			    }
			}

			drawBlocksCounter++;
        }
	}

	//draw horPlatform
    game1.draw(horPlatformSprite);

    //draw horPlatform
    game1.draw(vertPlatformSprite);

    //draw main sprite
    playerSprite.setTextureRect(playerFrames[playerFrameNumber]);

    game1.draw(playerSprite);

	for(int i = 0; i < TILES_ACROSS; i++)
    {
	    for(int j = 0; j < TILES_DOWN; j++)
		{
		    if(layer3[i][j] > 0 && !dead && !exitingLevel)
			{
			    //limit how many tiles are rendered to what is visible to the player
			    if(blocks[drawBlocksCounter].x > playerX - (SCREEN_WIDTH_PIXELS - 40 / 2)
			        && blocks[drawBlocksCounter].x < playerX + (SCREEN_WIDTH_PIXELS + 40 / 2)
			            && blocks[drawBlocksCounter].y > playerY - (SCREEN_HEIGHT_PIXELS - 40 / 2)
			                && blocks[drawBlocksCounter].y < playerY + (SCREEN_HEIGHT_PIXELS + 40 / 2))

			    {
			        game1.draw(blocks[drawBlocksCounter].sprite);
			    }
			}

			drawBlocksCounter++;
        }
	}

	//draw enemys
	for(int i = 0; i < NUMBER_OF_EACH_ENEMY; i++)
	{
	    if(!dead && !exitingLevel)
	    {
	        enemy1s.at(i).sprite.setPosition(enemy1s.at(i).xPos - offsetX + (enemy1s.at(i).speed * interpolation) * over_all_game_speed, enemy1s.at(i).yPos - offsetY);
	        game1.draw(enemy1s.at(i).sprite);
	    }
	}

	for(int i = 0; i < NUMBER_OF_EXPLOSIONS; i++)
	{
	    if(explosion.at(i).alive && !dead && !exitingLevel)
	    {
	        game1.draw(explosion.at(i).sprite);
	    }
	}

	for(int i = 0; i < NUMBER_OF_PICKUPS; i++)
	{
	    if(pickups.at(i).alive  && !dead && !exitingLevel)
	    {
	        pickups.at(i).sprite.setPosition(pickups.at(i).xPos - offsetX, pickups.at(i).yPos - offsetY);
	        game1.draw(pickups.at(i).sprite);
	    }
	}

	for(int i = 0; i < NUMBER_OF_COLLECTION_ANIMATIONS; i++)
	{
	    if(collection.at(i).alive && !dead && !exitingLevel)
	    {
	        collection.at(i).sprite.setPosition(collection.at(i).xPos - offsetX, collection.at(i).yPos - offsetY);
	        game1.draw(collection.at(i).sprite);
	    }
	}

    game1.draw(countDownTimerBg);

	//draw countdown timer
	game1.draw(countDownTimerText);

	if(showInGameMenu)
	{
	    game1.draw(menuBox);
	    game1.draw(gmenu_fpsdisplay);
	    game1.draw(gmenu_fToggleFScreen);
	    game1.draw(gmenu_fullscreendisplay);
	    game1.draw(gmenu_gamePausedText);
	    game1.draw(gmenu_pressEscText);
	    game1.draw(gmenu_qForTitleText);
	    game1.draw(gmenu_sChangeFPS);
	    game1.draw(gmenu_vsyncdisplay);
	    game1.draw(gmenu_vToggleVsync);
	}

	//draw level exit
	levelExit.sprite.setPosition(levelExit.xPos - offsetX, levelExit.yPos - offsetY);

	if(!dead)
	{
	    game1.draw(levelExit.sprite);
	}

	//debug

	/*sf::RectangleShape p1;
	p1.setFillColor(sf::Color::White);
	p1.setSize(sf::Vector2f(1, 1));
	p1.setPosition(point1.x - offsetX, point1.y - offsetY);
	game1.draw(p1);

	sf::RectangleShape p2;
	p2.setFillColor(sf::Color::White);
	p2.setSize(sf::Vector2f(1, 1));
	p2.setPosition(point2.x - offsetX, point2.y - offsetY);
	game1.draw(p2);

	sf::RectangleShape p3;
	p3.setFillColor(sf::Color::White);
	p3.setSize(sf::Vector2f(1, 1));
	p3.setPosition(point3.x - offsetX, point3.y - offsetY);
	game1.draw(p3);

    sf::RectangleShape p4;
	p4.setFillColor(sf::Color::White);
	p4.setSize(sf::Vector2f(1, 1));
	p4.setPosition(point4.x - offsetX, point4.y - offsetY);
	game1.draw(p4);

    sf::RectangleShape p5;
	p5.setFillColor(sf::Color::White);
	p5.setSize(sf::Vector2f(1, 1));
	p5.setPosition(point5.x - offsetX, point5.y - offsetY);
	game1.draw(p5);

    sf::RectangleShape p6;
	p6.setFillColor(sf::Color::White);
	p6.setSize(sf::Vector2f(1, 1));
	p6.setPosition(point6.x - offsetX, point6.y - offsetY);
	game1.draw(p6);

    sf::RectangleShape p7;
	p7.setFillColor(sf::Color::White);
	p7.setSize(sf::Vector2f(1, 1));
	p7.setPosition(point7.x - offsetX, point7.y - offsetY);
	game1.draw(p7);

    sf::RectangleShape p8;
	p8.setFillColor(sf::Color::White);
	p8.setSize(sf::Vector2f(1, 1));
	p8.setPosition(point8.x - offsetX, point8.y - offsetY);
	game1.draw(p8);

	sf::RectangleShape p5A;
	p5A.setFillColor(sf::Color::White);
	p5A.setSize(sf::Vector2f(1, 1));
	p5A.setPosition(point5A.x - offsetX, point5A.y - offsetY);
	game1.draw(p5A);

	sf::RectangleShape p6A;
	p6A.setFillColor(sf::Color::White);
	p6A.setSize(sf::Vector2f(1, 1));
	p6A.setPosition(point6A.x - offsetX, point6A.y - offsetY);
	game1.draw(p6A);*/

    // Update the window
    game1.display();
}

string toString(int value)
{
    ostringstream stream;
    stream << value;
    return stream.str();
}

string toString(float value)
{
    ostringstream stream;
    stream << value;
    return stream.str();
}

void LoadLevel(string level)
{
    //first of all, reset any objects that may be active
    enemy1Counter = -1;
    pickupCounter = -1;
    countDownTimer = COUNTDOWN_TIMER_RESET;

    for(int i = 0; i < NUMBER_OF_EACH_ENEMY; i++)
    {
        if(enemy1s.at(i).alive)
        {
            enemy1s.at(i).speed = 2.0;
        }

        enemy1s.at(i).kill();
    }

    for(int i = 0; i < NUMBER_OF_PICKUPS; i++)
    {
        if(pickups.at(i).alive)
        {
            pickups.at(i).kill();
        }
    }

    //open level file
    ifstream myFile;

    const char *levelToOpen;

    levelToOpen = level.c_str();

    myFile.open(levelToOpen);

    string data;

    int intVal = 0;
    loadLevelCounter = 0;

    for(int i = 0; i < TILES_ACROSS; i++)
    {
        for(int j = 0; j < TILES_DOWN; j++)
        {
            getline(myFile, data, ',');
            intVal = atoi(data.c_str());
            layer1[i][j] = intVal;

            if(layer1[i][j] > 0)
            {
                blocks[loadLevelCounter].sprite.setTexture(tileImages[layer1[i][j]]);
            }

            blocks[loadLevelCounter].x = i * TILE_WIDTH;
            blocks[loadLevelCounter].y = j * TILE_HEIGHT;
            blocks[loadLevelCounter].sprite.setPosition(blocks[loadLevelCounter].x, blocks[loadLevelCounter].y);
            loadLevelCounter++;
        }
    }

    for(int i = 0; i < TILES_ACROSS; i++)
    {
        for(int j = 0; j < TILES_DOWN; j++)
        {
            getline(myFile, data, ',');
            intVal = atoi(data.c_str());
            layer2[i][j] = intVal;

            if(layer2[i][j] > 0)
            {
                blocks[loadLevelCounter].sprite.setTexture(tileImages[layer2[i][j]]);
            }

            blocks[loadLevelCounter].x = i * TILE_WIDTH;
            blocks[loadLevelCounter].y = j * TILE_HEIGHT;
            blocks[loadLevelCounter].sprite.setPosition(blocks[loadLevelCounter].x, blocks[loadLevelCounter].y);
            loadLevelCounter++;
        }
    }

    for(int i = 0; i < TILES_ACROSS; i++)
    {
        for(int j = 0; j < TILES_DOWN; j++)
        {
            getline(myFile, data, ',');
            intVal = atoi(data.c_str());
            layer3[i][j] = intVal;

            if(layer3[i][j] > 0)
            {
                blocks[loadLevelCounter].sprite.setTexture(tileImages[layer3[i][j]]);
            }

            blocks[loadLevelCounter].x = i * TILE_WIDTH;
            blocks[loadLevelCounter].y = j * TILE_HEIGHT;
            blocks[loadLevelCounter].sprite.setPosition(blocks[loadLevelCounter].x, blocks[loadLevelCounter].y);
            loadLevelCounter++;
        }
    }

    for(int i = 0; i < TILES_ACROSS; i++)
    {
        for(int j = 0; j < TILES_DOWN; j++)
        {
            getline(myFile, data, ',');
            intVal = atoi(data.c_str());
            layer4[i][j] = intVal;

            if(layer4[i][j] == 34) //enemy1
            {
                enemy1Counter++;
                enemy1s.at(enemy1Counter).alive = true;
                enemy1s.at(enemy1Counter).xPos = i * TILE_WIDTH;
                enemy1s.at(enemy1Counter).yPos = j * TILE_HEIGHT;
                enemy1s.at(enemy1Counter).sprite.setPosition(enemy1s.at(enemy1Counter).xPos, enemy1s.at(enemy1Counter).yPos);
            }

            if(layer4[i][j] == 35) //pickup
            {
                pickupCounter++;
                pickups.at(pickupCounter).alive = true;
                pickups.at(pickupCounter).xPos = i * TILE_WIDTH;
                pickups.at(pickupCounter).yPos = j * TILE_HEIGHT;
                pickups.at(pickupCounter).sprite.setPosition(pickups.at(pickupCounter).xPos, pickups.at(pickupCounter).yPos);
            }

            //if a player starting position hasnt been set, set the starting position to 50, 50
            //playerX = 50;
            //playerY = 50;

            if(layer4[i][j] == 36) //player starting position
            {
                playerX = i * TILE_WIDTH;
                playerY = (j * TILE_HEIGHT) - (PLAYER_HEIGHT - TILE_HEIGHT);
            }

            //if the exit has not been placed make sure the exit isnt set to previous position
            //levelExit.xPos = -500.0;
            //levelExit.yPos = -500.0;
            //levelExit.sprite.setPosition(levelExit.xPos, levelExit.yPos);

            if(layer4[i][j] == 38) //exit
            {
                levelExit.xPos = i * TILE_WIDTH;
                levelExit.yPos = j * TILE_HEIGHT;
                levelExit.sprite.setPosition(levelExit.xPos, levelExit.yPos);
            }

            playerSprite.setPosition(playerX, playerY);
        }
    }

    myFile.close();
}

bool Collision(sf::Sprite sprite1, sf::Sprite sprite2)
{
    return !(sprite2.getPosition().x > sprite1.getPosition().x + sprite1.getGlobalBounds().width
        || sprite2.getPosition().x + sprite2.getGlobalBounds().width < sprite1.getPosition().x
            || sprite2.getPosition().y > sprite1.getPosition().y + sprite1.getGlobalBounds().height
                || sprite2.getPosition().y + sprite2.getGlobalBounds().height < sprite1.getPosition().y);
}

bool vectorInSprite(sf::Vector2f vector1, sf::Sprite sprite2)
{
    return !(sprite2.getPosition().x > vector1.x || sprite2.getPosition().x + sprite2.getGlobalBounds().width < vector1.x
            || sprite2.getPosition().y > vector1.y || sprite2.getPosition().y + sprite2.getGlobalBounds().height < vector1.y);
}

void drawTitleScreen()
{
    game1.clear(sf::Color::Black);

    drawBlocksCounter = 0;

    //draw layer 1
    for(int i = 0; i < TILES_ACROSS; i++)
    {
	    for(int j = 0; j < TILES_DOWN; j++)
		{
		    if(layer1[i][j] > 0)
			{
			    //limit how many tiles are rendered to what is visible to the player
			    if(blocks[drawBlocksCounter].x > playerX - (SCREEN_WIDTH_PIXELS - 40 / 2)
			        && blocks[drawBlocksCounter].x < playerX + (SCREEN_WIDTH_PIXELS + 40 / 2)
			            && blocks[drawBlocksCounter].y > playerY - (SCREEN_HEIGHT_PIXELS - 40 / 2)
			                && blocks[drawBlocksCounter].y < playerY + (SCREEN_HEIGHT_PIXELS + 40 / 2))

			    {
			        game1.draw(blocks[drawBlocksCounter].sprite);
			    }
			}

			drawBlocksCounter++;
        }
	}

    //draw layer 2
	for(int i = 0; i < TILES_ACROSS; i++)
    {
	    for(int j = 0; j < TILES_DOWN; j++)
		{
		    if(layer2[i][j] > 0)
			{
			    //limit how many tiles are rendered to what is visible to the player
			    if(blocks[drawBlocksCounter].x > playerX - (SCREEN_WIDTH_PIXELS - 40 / 2)
			        && blocks[drawBlocksCounter].x < playerX + (SCREEN_WIDTH_PIXELS + 40 / 2)
			            && blocks[drawBlocksCounter].y > playerY - (SCREEN_HEIGHT_PIXELS - 40 / 2)
			                && blocks[drawBlocksCounter].y < playerY + (SCREEN_HEIGHT_PIXELS + 40 / 2))

			    {
			        //game1.draw(blocks[drawBlocksCounter].sprite);
			    }
			}

			drawBlocksCounter++;
        }
	}

	for(int i = 0; i < NUMBER_OF_LINES; i++)
	{
	    if(lines[i].active)
	    {
	        lines[i].sprite.setPosition(LINE_X_POS, lines[i].yPos + (LINE_SPEED * interpolation));
	    }

	    game1.draw(lines[i].sprite);
	}

	game1.draw(diagonaluniverseText);

	if(!showTitleScreenMenu)
	{
	    game1.draw(pressEnterText);
	    game1.draw(pressEscapeText);
	}

	if(showTitleScreenMenu)
	{
	    game1.draw(menuBox);
	    game1.draw(gmenu_fpsdisplay);
	    game1.draw(gmenu_fToggleFScreen);
	    game1.draw(gmenu_fullscreendisplay);
	    game1.draw(tmenu_menuText);
	    game1.draw(tmenu_pressEscText);
	    game1.draw(tmenu_qForOSText);
	    game1.draw(gmenu_sChangeFPS);
	    game1.draw(gmenu_vsyncdisplay);
	    game1.draw(gmenu_vToggleVsync);
	}

	game1.display();
}

void updateIntro()
{
    if(!finishedText)
    {
        if(slowDownText == 10)
        {
            //keytypeSound.play();
            amountOfTextToShow += 1;
            textProgressor++;

            if(introString.at(textProgressor) != '9' && introString.at(textProgressor) != '\n' && introString.at(textProgressor) != '.')
            {
                keytypeSound.play();
            }
            if(introString.at(textProgressor) != '9')
            {
                textLines[lineNumber].setString(introString.substr(lineStart, amountOfTextToShow));
            }

            if(introString.at(textProgressor) == '\n')
            {
                lineNumber++;
                lineStart = textProgressor;
                amountOfTextToShow = 0;
            }

            if(introString.at(textProgressor) == '9')
            {
                finishedText = true;
            }

            slowDownText = 0;
        }

        slowDownText++;
    }

    //when the text is finished, press enter to start the game
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Return) && returnPressed == false)
    {
        returnPressed = true;
        levelNumber++;
        LoadLevel(levelListVector.at(levelNumber));

        gameState = gamePlay;

        titleScrMusic.stop();
        inGameMusic.play();
    }

    if(!sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
    {
        returnPressed = false;
    }
}

void drawIntro()
{
    game1.clear(sf::Color::Black);

    for(int i = 0; i < NUMBER_OF_TEXT_LINES; i++)
    {
        game1.draw(textLines[i]);
    }

    game1.display();
}

void updateBadEnding()
{
    if(!finishedTextBE)
    {
        if(slowDownTextBE == 10)
        {
            //keytypeSound.play();
            amountOfTextToShowBE += 1;
            textProgressorBE++;

            if(badEndingString.at(textProgressorBE) != '4' && badEndingString.at(textProgressorBE) != '\n' && badEndingString.at(textProgressorBE) != '.')
            {
                keytypeSound.play();
            }
            if(badEndingString.at(textProgressorBE) != '4')
            {
                textLinesBE[lineNumberBE].setString(badEndingString.substr(lineStartBE, amountOfTextToShowBE));
            }

            if(badEndingString.at(textProgressorBE) == '\n')
            {
                lineNumberBE++;
                lineStartBE = textProgressorBE;
                amountOfTextToShowBE = 0;
            }

            if(badEndingString.at(textProgressorBE) == '4')
            {
                finishedTextBE = true;
            }

            slowDownTextBE = 0;
        }

        slowDownTextBE++;
    }

    if(finishedTextBE)
    {
        if(endingRobotOpacity > 0)
        {
            endingRobotOpacity--;
        }

        endingRobotSprite.setColor(sf::Color(255, 255, 255, endingRobotOpacity));
    }

    //press enter to return to the title screen
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Return) && returnPressed == false)
    {
        returnPressed = true;
        levelNumber++; //set level name to titleScr????
        LoadLevel(levelListVector.at(levelNumber));

        gameState = gamePlay; //??
    }

    if(!sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
    {
        returnPressed = false;
    }
}

void drawBadEnding()
{
    game1.clear(sf::Color(100, 100, 100));
    game1.draw(endingRobotSprite);

    if(finishedTextBE)
    {
        game1.draw(results);
    }

    for(int i = 0; i < NUMBER_OF_TEXT_LINESBE; i++)
    {
        game1.draw(textLinesBE[i]);
    }

    game1.display();
}

void updateGoodEnding()
{
    if(!finishedTextGE)
    {
        if(slowDownTextGE == 10)
        {
            //keytypeSound.play();
            amountOfTextToShowGE += 1;
            textProgressorGE++;

            if(goodEndingString.at(textProgressorGE) != '4' && goodEndingString.at(textProgressorGE) != '\n' && goodEndingString.at(textProgressorGE) != '.')
            {
                keytypeSound.play();
            }
            if(goodEndingString.at(textProgressorGE) != '4')
            {
                textLinesGE[lineNumberGE].setString(goodEndingString.substr(lineStartGE, amountOfTextToShowGE));
            }

            if(goodEndingString.at(textProgressorGE) == '\n')
            {
                lineNumberGE++;
                lineStartGE = textProgressorGE;
                amountOfTextToShowGE = 0;
            }

            if(goodEndingString.at(textProgressorGE) == '4')
            {
                finishedTextGE = true;
            }

            slowDownTextGE = 0;
        }

        slowDownTextGE++;
    }

    if(finishedTextGE)
    {
        if(endingRobotOpacity > 0)
        {
            endingRobotOpacity--;
        }

        if(endingHumanOpacity < 255)
        {
            endingHumanOpacity++;
        }

        endingHumanSprite.setColor(sf::Color(255, 255, 255, endingHumanOpacity));
        endingRobotSprite.setColor(sf::Color(255, 255, 255, endingRobotOpacity));
    }

    //press enter to return to the title screen
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Return) && returnPressed == false)
    {
        returnPressed = true;
        levelNumber++; //set level name to titleScr????
        LoadLevel(levelListVector.at(levelNumber));

        gameState = gamePlay; //??
    }

    if(!sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
    {
        returnPressed = false;
    }
}

void drawGoodEnding()
{
    game1.clear(sf::Color(100, 100, 100));
    game1.draw(endingHumanSprite);
    game1.draw(endingRobotSprite);

    if(finishedTextGE)
    {
        game1.draw(results);
    }

    for(int i = 0; i < NUMBER_OF_TEXT_LINESGE; i++)
    {
        game1.draw(textLinesGE[i]);
    }

    game1.display();
}

void toggleFullScreen()
{
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::F) && fPressed == false)
    {
        fPressed = true;
        fullScreenMode = !fullScreenMode;

        if(fullScreenMode)
        {
            game1.create(sf::VideoMode(SCREEN_WIDTH_PIXELS, SCREEN_HEIGHT_PIXELS), GAME_NAME, sf::Style::Fullscreen);
            gmenu_fullscreendisplay.setString("Full Screen: ON");
        }

        if(!fullScreenMode)
        {
            game1.create(sf::VideoMode(SCREEN_WIDTH_PIXELS, SCREEN_HEIGHT_PIXELS), GAME_NAME,sf::Style::Close);
            gmenu_fullscreendisplay.setString("Full Screen: OFF");
        }

        game1.setMouseCursorVisible(false);
     }

     if(!sf::Keyboard::isKeyPressed(sf::Keyboard::F))
     {
         fPressed = false;
     }
}

void toggleVSync()
{
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::V) && vPressed == false)
    {
        vPressed = true;

        vSyncOn = !vSyncOn;

        game1.setVerticalSyncEnabled(vSyncOn);

        if(!vSyncOn)
        {
            gmenu_vsyncdisplay.setString("VSync: OFF");
        }

        if(vSyncOn)
        {
            gmenu_vsyncdisplay.setString("VSync: ON");
        }
    }

    if(!sf::Keyboard::isKeyPressed(sf::Keyboard::V))
    {
        vPressed = false;
    }
}

void toggleFPS()
{
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::S) && sPressed == false)
    {
        sPressed = true;

        frameLimit += 10;

        if(frameLimit == 10 || frameLimit == 20)
        {
            frameLimit = 30;
        }

        if(frameLimit <= 120)
        {
            gmenu_fpsdisplay.setString("FPS limit: " + toString(frameLimit));
        }

        if(frameLimit > 120)
        {
            frameLimit = 0;
            gmenu_fpsdisplay.setString("FPS limit: OFF");
        }

        game1.setFramerateLimit(frameLimit);
    }

    if(!sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        sPressed = false;
    }
}

void resetVars()
{
    over_all_game_speed = 1.0;
    offsetX = -400.0;
    offsetY = -300.0;
    animationStep = 0.0;
    playerFrameNumber = 0;
    playerX = 40.0;
    playerY = 40.0;
    playerSpeedX = 0.0;
    playerSpeedY = 0.0;
    extraJumpHeight = 0.0;
    facingRight = true;
    jumping = true;
    maxMoveUp = 0;
    maxMoveDown = 0;
    maxMoveLeft = 0;
    maxMoveRight = 0;
    point1Collision = false;
    point2Collision = false;
    point3Collision = false;
    point4Collision = false;
    point5Collision = false;
    point6Collision = false;
    point7Collision = false;
    point8Collision = false;
    playFootSound = true;
    loadLevelCounter = 0;
    whichExplosionToDisplay = 0;
    whichCollectionToDisplay = 0;
    color1 = 227;
    color2 = 105;
    color3 = 2;
    enemy1AnimationStep = 0.0;
    ENEMY1_ANIMATION_SPEED = 9.0;
    enemy1FrameNumber = 0;
    pickupAnimationStep = 0.0;
    PICKUP_ANIMATION_SPEED = 20.0;
    pickupFrameNumber = 0;
    pickupCounter = -1;
    levelNumber = -1;
    tabPressed = false;
    anomelyIntensity = none;
    effectChange = 0;
    returnPressed = false;
    amountOfTextToShow = 0;
    finishedText = false;
    textProgressor = 0;
    slowDownText = 0;
    lineNumber = 0;
    lineStart = 0;
    secondsCounter = 0;
    countDownTimer = COUNTDOWN_TIMER_RESET;
    playerHurt = 0;
    invincibleTime = 0;
    showInGameMenu = false;
    showTitleScreenMenu = false;
    exitingLevel = false;
    timeBeforeNextLevelLoads = 120;

    for(int i = 0; i < NUMBER_OF_TEXT_LINES; i++)
    {
        textLines[i].setString("");
    }

    for(int i = 0; i < NUMBER_OF_TEXT_LINESBE; i++)
    {
        textLinesBE[i].setString("");
    }

    for(int i = 0; i < NUMBER_OF_TEXT_LINESGE; i++)
    {
        textLinesGE[i].setString("");
    }

    makeAllTilesWhite();

    amountOfTextToShowGE = 0;
    finishedTextGE = false;
    textProgressorGE = 0;
    slowDownTextGE = 0;
    lineNumberGE = 0;
    lineStartGE = 0;
    endingRobotOpacity = 255;
    endingHumanOpacity = 0;
    numberOfEnemysKilledByPlayer = 0;
    tempHolderForPlayerScore = 0;

    endingRobotSprite.setColor(sf::Color(255, 255, 255, endingRobotOpacity));
    endingHumanSprite.setColor(sf::Color(255, 255, 255, endingHumanOpacity));

    amountOfTextToShowBE = 0;
    finishedTextBE = false;
    textProgressorBE = 0;
    slowDownTextBE = 0;
    lineNumberBE = 0;
    lineStartBE = 0;
}

void makeAllTilesWhite()
{
    for(int i = 0; i < NUMBER_OF_TILES; i++)
    {
        if(blocks[i].sprite.getColor() != sf::Color::White)
        {
            blocks[i].sprite.setColor(sf::Color::White);
        }
    }
}
