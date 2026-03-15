#include <iostream>
#include <fstream>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>

using namespace sf;
using namespace std;

int screen_x = 1152;
int screen_y = 896;

enum GameState
{
    CHAR_SELECT,
    PLAYING
};

enum LevelID
{
    LEVEL_1,
    LEVEL_2
};

void display_level (RenderWindow& window, char**lvl, Texture& bgTex,Sprite& bgSprite,Texture& blockTexture,Sprite& blockSprite, const int height, const int width, const int cell_size);
void player_gravity (char** lvl, float& offset_y, float& velocityY, bool& onGround, const float& gravity, float& terminal_Velocity, float& player_x, float& player_y, const int cell_size, int& Pheight, int& Pwidth, bool& teleported, int height, int width, Sprite& PlayerSprite,Texture& PlayerTexture);
void player_horizontal_movement(char** lvl, float& player_x, float& player_y, const int cell_size, int& Pheight, int& Pwidth, int height, int width, Sprite& PlayerSprite, Texture& PlayerTexture, float current_speed);
void level1_platforms (char** lvl, int width, int height);
void level2_platforms (char** lvl, int width, int height);
void ghost_movement (char** lvl, int& ghost_x, int& ghost_y, int& dirX, const int cell_size, int& Gheight, int& Gwidth, int height, int width);
void skeleton_movement(char** lvl, float& offset_y, float& velocityY, bool& onGround, const float& gravity, float& terminal_Velocity, int& skeleton_x, int& skeleton_y, int& dirX, const int cell_size, int& Sheight, int& Swidth, int height, int width);
void invisible_man_movement(char** lvl, float& offset_y, float& velocityY, bool& onGround, const float& gravity, float& terminal_Velocity, int& im_x, int& im_y, int& dirX, const int cell_size, int& IMheight, int& IMwidth, int height, int width);

void vaccum_is_On(Sprite enemySprite[], bool captured[], int count, float player_x, float player_y, int Pwidth, int Pheight, int PlayerDirection_X, int PlayerDirection_Y, float VaccumRange, float VaccumPower, float VaccumRadius, char enemy_stored[], int maxCapacity, char enemy_type,float& Vaccum_Suck_X,float& Vaccum_Suck_Y);
void draw_vacuum(RenderWindow& window,float Vaccum_Suck_X, float Vaccum_Suck_Y, int PlayerDirection_X, int PlayerDirection_Y,Sprite& VaccumSprite1, Sprite& VaccumSprite2, Sprite& VaccumSprite3,Sprite& VaccumSprite4);
void getThrowDirection(int PlayerDirection_X, int PlayerDirection_Y, int &dx, int &dy);
void moveRollingEnemyUpOnePlatform(char** lvl, int width, int height, int cell_size,int &ex, int &ey, int eW, int eH);
void moveRollingEnemyDownOnePlatform(char** lvl, int width, int height, int cell_size,int &ex, int &ey, int eW, int eH);
void updateRollingEnemies(char** lvl, int width, int height, int cell_size, int ghost_x[], int ghost_y[], int ghostDir[], bool GhostCaptured[], bool GhostRolling[], float GhostRollDist[], int g, int skeleton_x[], int skeleton_y[], int skeletonDir[], bool SkeletonCaptured[], bool SkeletonRolling[], float SkeletonRollDist[], int s, int invisible_man_x[], int invisible_man_y[], int invisible_manDir[], bool InvisibleCaptured[], bool InvisibleRolling[], float InvisibleRollDist[], int im, int Gwidth, int Gheight, int Swidth, int Sheight, int IMwidth, int IMheight, bool W_T_edge, bool S_T_edge, Sprite ghostSprite[], Sprite skeletonSprite[], Sprite invisible_manSprite[]);
void ThrowOneEnemy(char enemy_stored[], int maxCapacity, bool GhostCaptured[], const int g, bool SkeletonCaptured[], const int s, bool InvisibleCaptured[], const int im, int ghost_x[], int ghost_y[], int ghostDir[], int skeleton_x[], int skeleton_y[], int skeletonDir[], int invisible_man_x[], int invisible_man_y[], int invisible_manDir[], float player_x, float player_y, int PlayerDirection_X, int PlayerDirection_Y, bool GhostRolling[], float GhostRollDist[], bool SkeletonRolling[], float SkeletonRollDist[], bool InvisibleRolling[], float InvisibleRollDist[]);
void ThrowAllEnemies(char enemy_stored[], int maxCapacity, bool GhostCaptured[], const int g, bool SkeletonCaptured[], const int s, bool InvisibleCaptured[], const int im, int ghost_x[], int ghost_y[], int ghostDir[], int skeleton_x[], int skeleton_y[], int skeletonDir[], int invisible_man_x[], int invisible_man_y[], int invisible_manDir[], float player_x, float player_y, int PlayerDirection_X, int PlayerDirection_Y, bool GhostRolling[], float GhostRollDist[], bool SkeletonRolling[], float SkeletonRollDist[], bool InvisibleRolling[], float InvisibleRollDist[]);
void collision(RenderWindow& window, float& player_x, float& player_y, int g, int ghost_x[], int ghost_y[], bool GhostCaptured[], bool GhostRolling[], int s, int skeleton_x[], int skeleton_y[], bool SkeletonCaptured[], bool SkeletonRolling[], int im, int invisible_man_x[], int invisible_man_y[], bool InvisibleCaptured[], bool InvisibleRolling[], Sprite& PlayerSprite, int& lives);
void powerup (RenderWindow& window, char** lvl, int height, int width, int cell_size, float player_x, float player_y, int Pwidth, int Pheight, float& speed, float& VaccumRange, float& VaccumRadius, int& lives);
void loadLevel(LevelID level, char** lvl, int width, int height, int cell_size, int g, int ghost_x[], int ghost_y[], int ghostDir[], Sprite ghostSprite[], int s, int skeleton_x[], int skeleton_y[], float skeleton_Vy[], bool skeletonOnGround[], int skeletonDir[], Sprite skeletonSprite[], int im, int invisible_man_x[], int invisible_man_y[], float invisible_man_Vy[], bool invisible_manOnGround[], int invisible_manDir[], Sprite invisible_manSprite[], Texture& ghostTex, Texture& skeletonTex, Texture& invisible_manTex, float& player_x, float& player_y);
bool isSolid(char c);
bool findSlopeTile(char** lvl, int height, int width, float player_x, int Pwidth, int cell_size, int rowStart, int rowEnd, int& outRow, int& outCol);
void addRandomPlatformChunks(char** lvl, int width, int height);

int main()
{

    RenderWindow window(VideoMode(screen_x, screen_y), "Tumble-POP", Style::Resize);
    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(60);

    GameState state = CHAR_SELECT;
    LevelID currentLevel = LEVEL_1;

    //level specifics
    const int cell_size = 64;
    const int height = 14;
    const int width = 18;
    char** lvl;

    //creating level array
    lvl = new char* [height];
    for (int i = 0; i < height; i += 1)
    {
        lvl[i] = new char[width];
        for (int j = 0; j < width; j += 1)
            lvl[i][j] = ' ';  
    }
          
    //level and background textures and sprites
    Texture bgTex;
    Sprite bgSprite;
    Texture blockTexture;
    Sprite blockSprite;

    bgTex.loadFromFile("Asset/Sprites/bg.png");
    bgSprite.setTexture(bgTex);
    bgSprite.setPosition(0,0);

    blockTexture.loadFromFile("Asset/Sprites/block1.png");
    blockSprite.setTexture(blockTexture);
   
    srand(time(0));
        
    //GHOSTS
    const int g = 8;
    Texture ghostTex;
    Sprite ghostSprite[g];
    int ghost_x[g];
    int ghost_y[g];    
    int ghostDir[g];      //-1 means left, +1 means right
    int Gwidth = 60;
    int Gheight = 60;    
    ghostTex.loadFromFile("Asset/Sprites/ghost.png"); 

    //SKELETONS
    const int s = 4;
    Texture skeletonTex;
    Sprite skeletonSprite[s];
    int skeleton_x[s];
    int skeleton_y[s];
    float skeleton_Vy[s];
    bool skeletonOnGround[s];
    int skeletonDir[s];
    int Sheight = 60;
    int Swidth = 60;    
    skeletonTex.loadFromFile("Asset/Sprites/skeleton.png");

    //INVISIBLE MEN
    const int im = 3;
    Texture invisible_manTex;
    Sprite invisible_manSprite[im];
    int invisible_man_x[im];
    int invisible_man_y[im];
    float invisible_man_Vy[im];
    bool invisible_manOnGround[im];
    int invisible_manDir[im];
    int IMheight = 60;
    int IMwidth = 60;
    invisible_manTex.loadFromFile("Asset/Sprites/invisible_man.png");    
    bool invisible_manVisible[im];
    int invisible_manStateTimer[im];
 

    //CHELNOVS
    const int c=4;
    Texture chelnovTex;
    Sprite chelnovSprite[c];
    int chelnov_x[c];
    int chelnov_y[c];
    float chelnov_Vy[c];
    bool chelnovOnGround[c];
    int chelnovDir[c];
    int Cheight = 60;
    int Cwidth = 60;    
    chelnovTex.loadFromFile("Asset/Sprites/chelnov.png");

    //Music initialisation
    Music lvlMusic;
    lvlMusic.openFromFile("Asset/Sound/mus.ogg");
    lvlMusic.setVolume(20);
    lvlMusic.play();
    lvlMusic.setLoop(true);

    //PLAYER DATA
    float player_x = 530;
    float player_y = 100;
    float speed = 6;
    const float jumpStrength = -17; // Initial jump velocity
    const float gravity = 1;  // Gravity acceleration
    
    bool isJumping = false;  //track and check if jumping
    bool up_collide = false;
    bool left_collide = false;
    bool right_collide = false;

    Texture PlayerTexture;
    Sprite PlayerSprite;
    
    bool onGround = false;
    float offset_x = 0;
    float offset_y = 0;
    float velocityY = 0;
    float terminal_Velocity = 20;
    
    int PlayerHeight = 60;
    int PlayerWidth = 60;

    bool up_button = false;
    bool teleported = false;        //for jump teleportation logic
    bool up_key_pressed = false;
    
    //vaccum kay variables and stuff
    
    bool GhostCaptured[g]={false};
    bool SkeletonCaptured[s]={false};
    bool InvisibleCaptured[im] = {false};

    float VaccumRange=200; 
    const int arrsize=5;
    float Vaccum_Suck_X=0;
    float Vaccum_Suck_Y=0;
    bool T_is_pressed = false;     //for jump trigger just like the jump logic

    bool GhostRolling[g]={false};  //check if the ghost is thrown or not
    float GhostRollDist[g]={0};     //check how far this ghost is rolled

    bool SkeletonRolling[s]={false};    
    float SkeletonRollDist[s]={0};   

    bool InvisibleRolling[im]  = {false};
    float InvisibleRollDist[im] = {0};

    const float ROLL_SPEED =8;//speed of rolling enemies
    
    char enemy_stored[arrsize]={0};
    //vaccums sprites of different kinds
    Texture VaccumTexture1;
    Sprite  VaccumSprite1;

    Texture VaccumTexture2;
    Sprite VaccumSprite2;

    Texture VaccumTexture3;
    Sprite VaccumSprite3;

    Texture VaccumTexture4;
    Sprite VaccumSprite4;

    Texture GhostRelease;
    Sprite GhostRSprite;

    Texture SkeletonRelease;
    Sprite SkeletonRSprite;

    Texture InvisibleManRelease;
    Sprite InvisibleManRSprite;
    
    VaccumTexture1.loadFromFile("Asset/Sprites/vacuum1.png");
    VaccumTexture2.loadFromFile("Asset/Sprites/vacuum2.png");
    VaccumTexture3.loadFromFile("Asset/Sprites/vacuum3.png");
    VaccumTexture4.loadFromFile("Asset/Sprites/vacuum4.png");

    GhostRelease.loadFromFile("Asset/Sprites/ghost_roll.png");
    SkeletonRelease.loadFromFile("Asset/Sprites/skeleton_roll.png");
    InvisibleManRelease.loadFromFile("Asset/Sprites/invisible_man_roll.png");

    VaccumSprite1.setTexture(VaccumTexture1);
    VaccumSprite1.setScale(1,1);

    VaccumSprite2.setTexture(VaccumTexture2);
    VaccumSprite2.setScale(1,1);

    VaccumSprite3.setTexture(VaccumTexture3);
    VaccumSprite3.setScale(1,1);

    VaccumSprite4.setTexture(VaccumTexture4);
    VaccumSprite4.setScale(1, 1);

    GhostRSprite.setTexture(GhostRelease);
    GhostRSprite.setScale(1,1);

    SkeletonRSprite.setTexture(SkeletonRelease);
    SkeletonRSprite.setScale(1.2,1.2);

    InvisibleManRSprite.setTexture(InvisibleManRelease);
    InvisibleManRSprite.setScale(1,1);

    // Load level 1
    loadLevel(currentLevel, lvl, width, height, cell_size, g, ghost_x, ghost_y, ghostDir, ghostSprite, s, skeleton_x, skeleton_y, skeleton_Vy, skeletonOnGround, skeletonDir, skeletonSprite, im, invisible_man_x, invisible_man_y, invisible_man_Vy, invisible_manOnGround, invisible_manDir, invisible_manSprite, ghostTex, skeletonTex, invisible_manTex, player_x, player_y);

    //Menu
    Font font;
    font.loadFromFile("Asset/Font/font.ttf"); 

    Text gameTitle;
    gameTitle.setFont(font);
    gameTitle.setString("  TUMBLEPOP");
    gameTitle.setCharacterSize(60);
    gameTitle.setPosition(230, 100);

    Text charTitle;
    charTitle.setFont(font);
    charTitle.setString("CHOOSE YOUR CHARACTER");
    charTitle.setCharacterSize(40);
    charTitle.setPosition(230, 250);

    Text yellowChar;
    yellowChar.setFont(font);
    yellowChar.setString("YELLOW TUMBLEPOPPER\n1.5x Speed\nNormal Vacuum");
    yellowChar.setCharacterSize(30);
    yellowChar.setPosition(260, 360);

    Text greenChar;
    greenChar.setFont(font);
    greenChar.setString("GREEN TUMBLEPOPPER\nNormal Speed\n1.2x Vacuum Power & Width");
    greenChar.setCharacterSize(30);
    greenChar.setPosition(260, 480);
    //track the keys and data for menu 
    int selectedChar = 0;
    bool upHeld = false;
    bool downHeld = false;
    bool enterHeld = false;

    float player_speed = 6;
    float current_VacPower = 25;
    float current_VacRadius = 100;
    float base_player_speed = player_speed;
    float base_VacRadius = current_VacRadius;
    float base_VacPower = current_VacPower;
   
    Event ev;

    //Menu Sprites
    Texture menuYellowTex;
    Sprite menuYellowSprite;
    menuYellowTex.loadFromFile("Asset/Sprites/yellow_tumblepopper.png");
    menuYellowSprite.setTexture(menuYellowTex);
    menuYellowSprite.setScale(1, 0.9); 
    menuYellowSprite.setPosition(180, 370); //appear at left side 

    Texture menuGreenTex;
    Sprite menuGreenSprite;
    menuGreenTex.loadFromFile("Asset/Sprites/player.png");
    menuGreenSprite.setTexture(menuGreenTex);
    menuGreenSprite.setScale(2.3,2.3);
    menuGreenSprite.setPosition(180, 500); //appaer at left side 

    // move to right 
    static int idleFrames = 0;
    const int IDLE_THRESHOLD_FRAMES = 120; // 2 s mai appearence

    //main loop
    while (window.isOpen())
    {

        while (window.pollEvent(ev))
        {
            if (ev.type == Event::Closed) 
            {
                window.close();
            }

            if (ev.type == Event::KeyPressed)
            {
            }

        }

        //pressing escape to close
        if (Keyboard::isKeyPressed(Keyboard::Escape))
        {
            window.close();
        }

        //character selection 
        if (state == CHAR_SELECT)
        {
            if (Keyboard::isKeyPressed(Keyboard::Up))
            {
                if (!upHeld)
                {
                    selectedChar--;
                    if (selectedChar < 0) 
                        selectedChar = 1;
                }
                upHeld = true;
            }
            else upHeld = false;

            if (Keyboard::isKeyPressed(Keyboard::Down))
            {
                if (!downHeld)
                {
                    selectedChar++;
                    if (selectedChar > 1)
                     selectedChar = 0;
                }
                downHeld = true;
            }
            else downHeld = false;

            if (Keyboard::isKeyPressed(Keyboard::Enter))
            {
                if (!enterHeld)
                {
                    if (selectedChar == 0)
                    {
                        // Yellow TumblePopper
                        player_speed = 9; 
                        current_VacPower = 25;
                        current_VacRadius = 100;
                        base_player_speed = player_speed;
                        base_VacPower = current_VacPower;
                        base_VacRadius = current_VacRadius;

                        PlayerTexture.loadFromFile("Asset/Sprites/yellow_tumblepopper.png");
                        PlayerSprite.setTexture(PlayerTexture);
                        PlayerSprite.setScale(1, 1);
                        PlayerSprite.setPosition(player_x, player_y);

                    }
                    else
                    {
                        // Green Tumblepopper
                        player_speed = 6;
                        current_VacPower = 30;
                        current_VacRadius = 120;
                        base_player_speed = player_speed;
                        base_VacPower = current_VacPower;
                        base_VacRadius = current_VacRadius;

                        PlayerTexture.loadFromFile("Asset/Sprites/player.png");
                        PlayerSprite.setTexture(PlayerTexture);
                        PlayerSprite.setScale(2.2, 2.2);
                        PlayerSprite.setPosition(player_x, player_y);
                    }
                    state = PLAYING;
                }
                enterHeld = true;
            }
            else enterHeld = false;

            window.clear();
            window.draw(gameTitle);   
            window.draw(charTitle);

            if (selectedChar == 0) 
                yellowChar.setFillColor(Color::Yellow);
            
            else
             yellowChar.setFillColor(Color::White);

            if (selectedChar == 1)
             greenChar.setFillColor(Color::Green);
            
            else
             greenChar.setFillColor(Color::White);

            window.draw(yellowChar);
            window.draw(greenChar);
            window.draw(menuYellowSprite);
            window.draw(menuGreenSprite);
            window.display();
            continue;
        }
        
        //triggered only when button goes from up to down means button duba ke neche gaya
        bool jump_now = Keyboard::isKeyPressed(Keyboard::Up);
        if (jump_now && !up_key_pressed && onGround)
        {
            velocityY = jumpStrength;
            onGround = false;
            teleported = false; //allow teleport once in this new jump
        }
        up_key_pressed = jump_now;

        //first check the bounds for x axis i.e the horizontal 
        if (player_x < 0)
            player_x = 0;
        if (player_x > width*cell_size - PlayerWidth)
            player_x = width*cell_size - PlayerWidth;

        //then for y axis
        if (player_y < 0)
            player_y = 0;
        if (player_y > height*cell_size - PlayerHeight)
            player_y = height*cell_size - PlayerHeight;


        window.clear();

        static int lives = 3; 
        collision(window, player_x, player_y, g, ghost_x, ghost_y, GhostCaptured, GhostRolling, s, skeleton_x, skeleton_y, SkeletonCaptured, SkeletonRolling, im, invisible_man_x, invisible_man_y, InvisibleCaptured, InvisibleRolling, PlayerSprite, lives);

        if (lives <= 0)
           window.close();

        display_level(window, lvl, bgTex, bgSprite, blockTexture, blockSprite, height, width, cell_size);
        player_gravity(lvl, offset_y, velocityY, onGround, gravity, terminal_Velocity, player_x, player_y, cell_size, PlayerHeight, PlayerWidth, teleported, height, width, PlayerSprite, PlayerTexture);
        player_horizontal_movement(lvl, player_x, player_y, cell_size, PlayerHeight, PlayerWidth, height, width, PlayerSprite, PlayerTexture, player_speed);        

        {
            int feetRow = static_cast<int>(player_y + PlayerHeight) / cell_size;
            if (feetRow < 0) feetRow = 0;
            if (feetRow > height - 1) feetRow = height - 1;

            int slopeRow = -1, slopeCol = -1;
            bool onSlopeNow = findSlopeTile(lvl, height, width,
                                            player_x, PlayerWidth, cell_size,
                                            feetRow, feetRow,
                                            slopeRow, slopeCol);

            bool anyMoveKey =(Keyboard::isKeyPressed(Keyboard::Left)  ||Keyboard::isKeyPressed(Keyboard::Right) ||Keyboard::isKeyPressed(Keyboard::Up)    ||Keyboard::isKeyPressed(Keyboard::Down));

            if (onGround && velocityY == 0.0f && !anyMoveKey && onSlopeNow)
            {
                idleFrames++;
            }
            else
            {
                idleFrames = 0;
            }

            if (idleFrames > IDLE_THRESHOLD_FRAMES && onSlopeNow)
            {
                float nextX = player_x + player_speed;
                int rightTile   = static_cast<int>(nextX + PlayerWidth) / cell_size;
                int topTileY    = static_cast<int>(player_y) / cell_size;
                int bottomTileY = static_cast<int>(player_y + PlayerHeight - 1) / cell_size;

                if (rightTile < 0)            rightTile = 0;
                if (rightTile > width - 1)    rightTile = width - 1;
                if (topTileY < 0)             topTileY = 0;
                if (topTileY > height - 1)    topTileY = height - 1;
                if (bottomTileY < 0)          bottomTileY = 0;
                if (bottomTileY > height - 1) bottomTileY = height - 1;

                char right_mid     = lvl[(topTileY + bottomTileY) / 2][rightTile];
                char top_right     = lvl[topTileY][rightTile];
                char bottom_right  = lvl[bottomTileY][rightTile];

                if (!isSolid(top_right) && !isSolid(bottom_right) && !isSolid(right_mid))
                {
                    player_x = nextX;
                }

                int w = static_cast<int>(PlayerTexture.getSize().x);
                int h = static_cast<int>(PlayerTexture.getSize().y);
                PlayerSprite.setTextureRect(IntRect(w, 0, -w, h));

                idleFrames = IDLE_THRESHOLD_FRAMES;
            }
        }

        PlayerSprite.setPosition(player_x, player_y);       

        int maxCapacity = (currentLevel == LEVEL_1 ? 3 : 5);

        //W A S D Keys for vaccum
        int PlayerDirection_X=0;
        int PlayerDirection_Y=0;
        if (Keyboard::isKeyPressed(Keyboard::W))
            {   PlayerDirection_X=0;
                PlayerDirection_Y=-1;
             }
        if (Keyboard::isKeyPressed(Keyboard::A))
        {
            PlayerDirection_X=-1;
            PlayerDirection_Y=0;
            PlayerSprite.setTextureRect(IntRect(0,0,PlayerTexture.getSize().x,PlayerTexture.getSize().y));   
        }
        if (Keyboard::isKeyPressed(Keyboard::S))
        {
            PlayerDirection_X=0;
            PlayerDirection_Y=1;
        }
        if (Keyboard::isKeyPressed(Keyboard::D))
        {
            PlayerDirection_X=1;
            PlayerDirection_Y=0;
            PlayerSprite.setTextureRect(IntRect(PlayerTexture.getSize().x,0,-PlayerTexture.getSize().x, PlayerTexture.getSize().y));
        }


        bool VaccumOn=Keyboard::isKeyPressed(Keyboard::Space);
        if (VaccumOn)
        {
            vaccum_is_On(ghostSprite, GhostCaptured, g, player_x, player_y, PlayerWidth, PlayerHeight, PlayerDirection_X,PlayerDirection_Y, VaccumRange, current_VacPower, current_VacRadius, enemy_stored, maxCapacity, 'g',Vaccum_Suck_X,Vaccum_Suck_Y);
            vaccum_is_On(skeletonSprite,SkeletonCaptured, s, player_x, player_y, PlayerWidth, PlayerHeight, PlayerDirection_X, PlayerDirection_Y, VaccumRange, current_VacPower, current_VacRadius, enemy_stored, maxCapacity, 's',Vaccum_Suck_X,Vaccum_Suck_Y);
            vaccum_is_On(invisible_manSprite, InvisibleCaptured, im, player_x, player_y, PlayerWidth, PlayerHeight, PlayerDirection_X, PlayerDirection_Y, VaccumRange, current_VacPower, current_VacRadius, enemy_stored, maxCapacity, 'i', Vaccum_Suck_X, Vaccum_Suck_Y);
            draw_vacuum(window,Vaccum_Suck_X, Vaccum_Suck_Y,PlayerDirection_X, PlayerDirection_Y,VaccumSprite1, VaccumSprite2, VaccumSprite3, VaccumSprite4);
            
        }

        // checks for walls for passing them using vaccum throw
        static bool W_wasPressed = false;
        static bool S_wasPressed = false;

        bool W_now = Keyboard::isKeyPressed(Keyboard::W);
        bool S_now = Keyboard::isKeyPressed(Keyboard::S);
        bool T_now_roll = Keyboard::isKeyPressed(Keyboard::T);

        bool W_T_edge = (W_now && T_now_roll); //W and T is pressed same time 
        bool S_T_edge = (S_now && T_now_roll);//S and T is pressed same time

        W_wasPressed = W_now;
        S_wasPressed = S_now;



    
        bool T_now = Keyboard::isKeyPressed(Keyboard::T);
        if (T_now && !T_is_pressed)   //pressed this time and will rekease one enemy
        {
            ThrowOneEnemy(enemy_stored, maxCapacity, GhostCaptured, g, SkeletonCaptured, s, InvisibleCaptured, im, ghost_x, ghost_y, ghostDir, skeleton_x, skeleton_y, skeletonDir, invisible_man_x, invisible_man_y, invisible_manDir, player_x, player_y, PlayerDirection_X, PlayerDirection_Y, GhostRolling, GhostRollDist, SkeletonRolling, SkeletonRollDist, InvisibleRolling, InvisibleRollDist);
        }

        T_is_pressed = T_now;  //remember the situation like the jump and down logics

        if (Keyboard::isKeyPressed(Keyboard::Y))
        {
            ThrowAllEnemies(enemy_stored, maxCapacity, GhostCaptured, g, SkeletonCaptured, s, InvisibleCaptured, im, ghost_x, ghost_y, ghostDir, skeleton_x, skeleton_y, skeletonDir, invisible_man_x, invisible_man_y, invisible_manDir, player_x, player_y, PlayerDirection_X, PlayerDirection_Y, GhostRolling, GhostRollDist, SkeletonRolling, SkeletonRollDist, InvisibleRolling, InvisibleRollDist);        
        }

        updateRollingEnemies(lvl, width, height, cell_size, ghost_x, ghost_y, ghostDir, GhostCaptured, GhostRolling, GhostRollDist, g, skeleton_x, skeleton_y, skeletonDir, SkeletonCaptured, SkeletonRolling, SkeletonRollDist, s, invisible_man_x, invisible_man_y, invisible_manDir, InvisibleCaptured, InvisibleRolling, InvisibleRollDist, im, Gwidth, Gheight, Swidth, Sheight, IMwidth, IMheight, W_T_edge, S_T_edge, ghostSprite, skeletonSprite, invisible_manSprite);

       //move only the ghosts that are not captured.
      for (int i=0; i<g; i++)
      {   
          if (GhostCaptured[i]|| GhostRolling[i])     
               continue;  

         ghost_movement(lvl, ghost_x[i], ghost_y[i], ghostDir[i], cell_size, Gheight, Gwidth, height, width);
         ghostSprite[i].setPosition(ghost_x[i], ghost_y[i]);
      }

     //move only the skeletons that are not captured
     for (int i=0; i<s; i++)
     {
         if (SkeletonCaptured[i]|| SkeletonRolling[i])
             continue;
         skeleton_movement(lvl, offset_y, skeleton_Vy[i], skeletonOnGround[i], gravity, terminal_Velocity, skeleton_x[i], skeleton_y[i], skeletonDir[i], cell_size, Sheight, Swidth, height, width);
         skeletonSprite[i].setPosition(skeleton_x[i], skeleton_y[i]);
     }

    // move invisible men when level 2 is on
    if (currentLevel == LEVEL_2)
    {
        for (int i = 0; i < im; ++i)
        {
            invisible_man_movement(lvl, offset_y, invisible_man_Vy[i], invisible_manOnGround[i], gravity, terminal_Velocity, invisible_man_x[i], invisible_man_y[i], invisible_manDir[i], cell_size, IMheight, IMwidth, height, width);
            invisible_manSprite[i].setPosition(invisible_man_x[i], invisible_man_y[i]);
        }
    }



     window.draw(PlayerSprite);

     powerup(window, lvl, height, width, cell_size, player_x, player_y, PlayerWidth, PlayerHeight, player_speed, VaccumRange, current_VacRadius, lives);

     // lvel checks
     if (currentLevel == LEVEL_1)
     {
         bool anyGhostAlive = false;
         for (int i = 0; i < g; ++i)
         {
             if (!GhostCaptured[i] && !GhostRolling[i] && ghost_x[i] > -1000)
             {
                 anyGhostAlive = true;
                 break;
             }
         }

         bool anySkeletonAlive = false;
         for (int i = 0; i < s; ++i)
         {
             if (!SkeletonCaptured[i] && !SkeletonRolling[i] && skeleton_x[i] > -1000)
             {
                 anySkeletonAlive = true;
                 break;
             }
         }

         if (!anyGhostAlive && !anySkeletonAlive)
         {
             currentLevel = LEVEL_2;

             for (int i = 0; i < g; ++i)
             {
                 GhostCaptured[i] = false;
                 GhostRolling[i]  = false;
                 GhostRollDist[i] = 0;
                 ghost_x[i] = -5000;
                 ghost_y[i] = -5000;
             }
             for (int i = 0; i < s; ++i)
             {
                 SkeletonCaptured[i] = false;
                 SkeletonRolling[i]  = false;
                 SkeletonRollDist[i] = 0;
                 skeleton_Vy[i]      = 0;
                 skeletonOnGround[i] = false;
                 skeleton_x[i] = -5000;
                 skeleton_y[i] = -5000;
             }
             for (int i = 0; i < arrsize; ++i)
                 enemy_stored[i] = 0;

             lives = 3;
             player_speed = base_player_speed;
             current_VacPower = base_VacPower;
             current_VacRadius = base_VacRadius;
             VaccumRange = 200;

             loadLevel(currentLevel, lvl, width, height, cell_size, g, ghost_x, ghost_y, ghostDir, ghostSprite, s, skeleton_x, skeleton_y, skeleton_Vy, skeletonOnGround, skeletonDir, skeletonSprite, im, invisible_man_x, invisible_man_y, invisible_man_Vy, invisible_manOnGround, invisible_manDir, invisible_manSprite, ghostTex, skeletonTex, invisible_manTex, player_x, player_y);

             PlayerSprite.setPosition(player_x, player_y);
         }
     }

    //Ghosts appear
    for (int i = 0; i < g; i++)
    {
        if (GhostRolling[i])
        {
            GhostRSprite.setPosition(ghost_x[i], ghost_y[i]);
            window.draw(GhostRSprite);
        }
        else if (!GhostCaptured[i])
        {
            
            window.draw(ghostSprite[i]);
        }
    }

    //skeletons appear
    for (int i = 0; i < s; i++)
    {
        if (SkeletonRolling[i])
        {
            
            SkeletonRSprite.setPosition(skeleton_x[i], skeleton_y[i]);
            window.draw(SkeletonRSprite);
        }
        else if (!SkeletonCaptured[i])
        {
            
            window.draw(skeletonSprite[i]);
        }
    }
    
    // invisible men appear in Level 2
    if (currentLevel == LEVEL_2)
    {
        for (int i = 0; i < im; ++i)
        {
            if (InvisibleRolling[i])
            { 
                InvisibleManRSprite.setPosition(invisible_man_x[i], invisible_man_y[i]);
                window.draw(InvisibleManRSprite);
            }
            else if (!InvisibleCaptured[i])
            {
                window.draw(invisible_manSprite[i]);
            }
        }
    }


      window.display();
      }

    //stopping music and deleting level array
    lvlMusic.stop();
    for (int i = 0; i < height; i++)
        delete[] lvl[i];
        
    delete[] lvl;

    return 0;
}



void display_level(RenderWindow& window, char**lvl, Texture& bgTex, Sprite& bgSprite, Texture& blockTexture, Sprite& blockSprite, const int height, const int width, const int cell_size)
{
    window.draw(bgSprite);

    static Texture slantedTex;
    static Sprite slantedSprite;
    static bool slantedLoaded = false;

    if (!slantedLoaded)
    {
        slantedTex.loadFromFile("Asset/Sprites/Slanted.png");
        slantedSprite.setTexture(slantedTex);
        slantedLoaded = true;
    }

    for (int i = 0; i < height; i += 1)
    {
        for (int j = 0; j < width; j += 1)
        {
            if (lvl[i][j] == '#')
            {   blockSprite.setPosition(j * cell_size, i * cell_size);
                window.draw(blockSprite);   }
            else if (lvl[i][j] == '/')
            {
                slantedSprite.setPosition(j * cell_size, i * cell_size);
                window.draw(slantedSprite);
            }
        }
    }

}



void player_gravity(char** lvl, float& offset_y, float& velocityY, bool& onGround, const float& gravity, float& terminal_Velocity, float& player_x, float& player_y, const int cell_size, int& Pheight, int& Pwidth, bool& teleported, int height, int width, Sprite& PlayerSprite, Texture& PlayerTexture)
{
    offset_y = player_y; 

    velocityY += gravity;
    if (velocityY >= terminal_Velocity) 
      velocityY = terminal_Velocity;

    offset_y = player_y + velocityY;

    int leftX = static_cast<int>(player_x) / cell_size;
    int rightX = static_cast<int>(player_x + Pwidth) / cell_size;
    int midX = static_cast<int>(player_x + Pwidth / 2) / cell_size;
    int bottomY;
    bool downPressedNow = Keyboard::isKeyPressed(Keyboard::Down);

    if (velocityY > 0)
    {   
        bottomY = static_cast<int>(offset_y + Pheight) / cell_size;
       
        if (bottomY < 0)            
         bottomY = 0;

        if (bottomY > height-1)     
            bottomY = height-1;

        if (leftX < 0)               
            leftX = 0;

        if (leftX > width-1)   
              leftX = width-1;

        if (rightX < 0)
            rightX = 0;

        if (rightX > width-1)       
           rightX = width-1;

        if (midX < 0)                
            midX = 0;

        if (midX > width-1)          
            midX = width-1;

        int slopeRow;
        int slopeCol;
        bool slopeBelow = findSlopeTile(lvl, height, width, player_x, Pwidth, cell_size, bottomY - 1, bottomY, slopeRow, slopeCol);

        if (slopeBelow && !downPressedNow)
        {
            float feetX = player_x + Pwidth / 2.0f;
            float feetY = offset_y + Pheight;
            float tileLeft = slopeCol * cell_size;
            float localX = feetX - tileLeft;

            if (localX < 0)
                localX = 0;
            if (localX > static_cast<float>(cell_size))
                localX = static_cast<float>(cell_size);

            float tileBottom = (slopeRow + 1) * cell_size;
            float slopeY = tileBottom - localX;

            if (feetY >= slopeY - 2)
            {
                onGround = true;
                velocityY = 0;
                player_y = slopeY - Pheight;
                return;
            }
        }

        char bottom_left_down = lvl[bottomY][leftX];
        char bottom_mid_down = lvl[bottomY][midX];
        char bottom_right_down = lvl[bottomY][rightX];

        if (bottom_left_down == '#' || bottom_mid_down == '#' || bottom_right_down == '#')
        {
            onGround = true;
            velocityY = 0;
            player_y = bottomY * cell_size - Pheight;
        }
        else
        {
            onGround = false;
            player_y = offset_y;
        }
    }

    
    if (velocityY < 0)
    {
        int topY  = static_cast<int>(offset_y) / cell_size;
   
        if (topY < 0)               
            topY = 0;
        if (topY > height-1)        
            topY = height-1;

        if (leftX < 0)              
            leftX = 0;
        if (leftX > width-1)        
            leftX = width-1;

        if (rightX < 0)             
            rightX = 0;
        if (rightX > width-1)       
            rightX = width-1;

        if (midX < 0)               
            midX = 0;
        if (midX > width-1)         
            midX = width-1;

        char top_left_up = lvl[topY][leftX];
        char top_mid_up = lvl[topY][midX];
        char top_right_up = lvl[topY][rightX];

        bool hitCeiling = (top_left_up == '#' || top_mid_up == '#' || top_right_up == '#');

        if (hitCeiling)
        {
        
            bottomY =static_cast<int>(player_y+Pheight)/cell_size;

            if (bottomY<0) 
                bottomY = 0;
            if (bottomY>height-1)    
                bottomY= height-1;

            char bottom_left = lvl[bottomY][leftX];
            char bottom_mid = lvl[bottomY][midX];
            char bottom_right = lvl[bottomY][rightX];

            bool floorBelow = (bottom_left == '#' || bottom_mid == '#' || bottom_right == '#');
        
            bool doubleCeiling = false;
            if (topY > 0)
            {   int aboveRow = topY - 1;
                char above_left = lvl[aboveRow][leftX];
                char above_mid = lvl[aboveRow][midX];
                char above_right = lvl[aboveRow][rightX];


                if (above_left == '#'|| above_mid =='#'||above_right == '#')
                   doubleCeiling = true;   }
             
            if (!teleported && floorBelow && topY>0 && !doubleCeiling)
            {
                player_y = topY *cell_size - Pheight; 
                velocityY = 0;
                onGround = true;
                teleported = true;
            }
            else
            {   
                player_y  = (topY+1) *cell_size;    
                velocityY = 0;
                onGround  = false;   }
    
        }
    
        else
        {   
            player_y = offset_y;
            onGround = false;   }

    }
    
    
static bool downWasPressed = false;
bool downNow = Keyboard::isKeyPressed(Keyboard::Down);

if (downNow && !downWasPressed && onGround)
{
    int leftX  = static_cast<int>(player_x) / cell_size;
    int rightX = static_cast<int>(player_x + Pwidth) / cell_size;
    int midX   = static_cast<int>(player_x + Pwidth / 2) / cell_size;

    if (leftX < 0)            
        leftX = 0;

    if (leftX > width-1)      
        leftX = width-1;

    if (rightX < 0)           
        rightX = 0;

    if (rightX > width-1)     
        rightX = width-1;

    if (midX < 0)             
        midX = 0;

    if (midX > width-1)       
        midX = width-1;

    int currentBottomRow = static_cast<int>(player_y + Pheight) / cell_size;
    if (currentBottomRow < 0)         currentBottomRow = 0;
    if (currentBottomRow >= height)   currentBottomRow = height - 1;

    int targetRow = -1;
    for (int row = currentBottomRow + 1; row < height; ++row)
    {
        char bl = lvl[row][leftX];
        char bm = lvl[row][midX];
        char br = lvl[row][rightX];

        if (bl == '#' || bm == '#' || br == '#')
        {
            targetRow = row;
            break;
        }
    }
    int block_Present=0;
    block_Present=targetRow- currentBottomRow-1;

   
    if (targetRow != -1 && block_Present==1)
    {
        bool doubleFloor = false;
        if (targetRow +1 < height)
        {
            char bl2 = lvl[targetRow + 1][leftX];
            char bm2 = lvl[targetRow + 1][midX];
            char br2 = lvl[targetRow + 1][rightX];

            if (bl2 == '#' || bm2 == '#' || br2 == '#')
                doubleFloor = true;
        }

        if (!doubleFloor)
        {
            player_y  = targetRow * cell_size - Pheight;
            velocityY = 0;
            onGround  = true;
        }
    }
}

downWasPressed = downNow;

}    
    
void player_horizontal_movement(char** lvl, float& player_x, float& player_y, const int cell_size, int& Pheight, int& Pwidth, int height, int width, Sprite& PlayerSprite, Texture& PlayerTexture, float current_speed)
{
    //Horizontal movenment
    //Left
    if (Keyboard::isKeyPressed(Keyboard::Left))
    {
        float nextX = player_x - current_speed;
        int leftTile = static_cast<int>(nextX) / cell_size;
        int topTileY = static_cast<int>(player_y) / cell_size;
        int bottomTileY = static_cast<int>(player_y + Pheight - 1) / cell_size;
        
        if (leftTile < 0)              leftTile = 0;
        if (leftTile > width-1)        leftTile = width-1;

        if (topTileY < 0)              topTileY = 0;
        if (topTileY > height-1)       topTileY = height-1;

        if (bottomTileY < 0)           bottomTileY = 0;
        if (bottomTileY > height-1)    bottomTileY = height-1;

        char left_mid = lvl[(topTileY + bottomTileY) / 2][leftTile];
        char top_left = lvl[topTileY][leftTile];
        char bottom_left = lvl[bottomTileY][leftTile];

        if (!isSolid(top_left) && !isSolid(bottom_left) && !isSolid(left_mid))
            player_x = nextX;
        PlayerSprite.setTextureRect(IntRect(0,0,PlayerTexture.getSize().x,PlayerTexture.getSize().y));    
    }

    // RIGHT
    if (Keyboard::isKeyPressed(Keyboard::Right))
    {
        float nextX = player_x + current_speed;
        int rightTile = static_cast<int>(nextX + Pwidth) / cell_size;
        int topTileY = static_cast<int>(player_y) / cell_size;
        int bottomTileY = static_cast<int>(player_y + Pheight - 1) / cell_size;
        
        if (rightTile < 0)              rightTile = 0;
        if (rightTile > width-1)        rightTile = width-1;

        if (topTileY < 0)               topTileY = 0;
        if (topTileY > height-1)        topTileY = height-1;

        if (bottomTileY < 0)            bottomTileY = 0;
        if (bottomTileY > height-1)     bottomTileY = height-1;

        char right_mid = lvl[(topTileY + bottomTileY) / 2][rightTile];
        char top_right = lvl[topTileY][rightTile];
        char bottom_right = lvl[bottomTileY][rightTile];

        if (!isSolid(top_right) && !isSolid(bottom_right) && !isSolid(right_mid))
            player_x = nextX;
       /* it starts from the right side and gives a flip by -1 and the flip equals to ts height.
        playerSprite.setTextureRect(IntRect(x(left),y(top), width, height)); this is the genearl syntax. 
         here x and y are top left coordinates of the shape and widht height uski sides        */
        PlayerSprite.setTextureRect(IntRect(PlayerTexture.getSize().x,0,-PlayerTexture.getSize().x, PlayerTexture.getSize().y));
         
         
    }        
    
    //BOUNDS CHECK
    if (player_x < 0)
      player_x = 0;
    if (player_x > (width * cell_size - Pwidth))
      player_x = width * cell_size - Pwidth;
    if (player_y < 0)
      player_y = 0;
    if (player_y > (height * cell_size - Pheight))
      player_y = height * cell_size - Pheight;    
    
}



void level1_platforms(char** lvl, int width, int height)
{
    for(int i=0; i<width; i++ )
    lvl[0][i] = '#';
    
    for(int i=2; i<width-2; i++ )
    lvl[3][i] = '#';
    
    for (int i=width/2-2; i<width/2+2; i++)
    lvl[4][i] = '#';
    
    for(int i=0; i<4; i++ )
    lvl[5][i] = '#';

    for(int i=width-4; i<width; i++ )
    lvl[5][i] = '#';

    for(int i=2; i<width-2; i++ )
    lvl[7][i] = '#';

     for (int i=5; i<=height-5; i++)
       for (int j=width/2-2; j<width/2+2; j++) 
       lvl[i][j] = '#';
    
    for(int i=0; i<4; i++ )
    lvl[height-5][i] = '#';

    for (int i=width/2-2; i<width/2+2; i++)
    lvl[height-4][i] = '#';

    for(int i=width-4; i<width; i++ )
    lvl[height-5][i] = '#';
    
    for(int i=2; i<width-2; i++ )
    lvl[height-3][i] = '#';
    
    for(int i=0; i<width; i++ )
    lvl[height-1][i] = '#';    
}

void level2_platforms(char** lvl, int width, int height)
{
    // 1. Clear everything
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            lvl[i][j] = ' ';

    // boundaries
    for (int j = 0; j < width; j++)
        lvl[height - 1][j] = '#';    //floor
    for (int i = 0; i < height; i++)
        lvl[i][0] = '#';             //left wall
    for (int i = 0; i < height; i++)
        lvl[i][width - 1] = '#';     //rightwall

    //slanted platform
    for (int i = 0; i < 9; ++i)
    {
        int r = 1 + i;
        int c = 4 + i;
        if (r < height && c < width)
            lvl[r][c] = '/';
    }

    //some fixed platforms to integrate
    for (int i = 1; i < 4; ++i)
    {
        lvl[1][i] = '#';
    }

    for (int i = 14; i < 17; ++i)
    {
        if (i < width)
            lvl[11][i] = '#';
    }

    // Left Side Steps
    lvl[11][1] = '#'; lvl[11][2] = '#'; lvl[11][3] = '#'; lvl[11][4] = '#';
    lvl[ 9][1] = '#'; lvl[ 9][2] = '#'; lvl[ 9][3] = '#'; lvl[ 9][4] = '#';
    lvl[ 7][1] = '#'; lvl[ 7][2] = '#'; lvl[ 7][3] = '#'; lvl[ 7][4] = '#';

    for (int i = 1; i < 4; ++i)
    {
        lvl[3][i] = '#';
        lvl[5][i] = '#';
    }

    
    lvl[11][6] = '#'; lvl[11][7] = '#'; lvl[11][5] = '#'; lvl[11][4] = '#';

    // Right Side
    if (8 < height)
    {
        if (15 < width) lvl[8][15] = '#';
        if (16 < width) lvl[8][16] = '#';
        if (17 < width) lvl[8][17] = '#';
    }

    //add random blocks
    addRandomPlatformChunks(lvl, width, height);
}

void loadLevel(LevelID level, char** lvl, int width, int height, int cell_size, int g, int ghost_x[], int ghost_y[], int ghostDir[], Sprite ghostSprite[], int s, int skeleton_x[], int skeleton_y[], float skeleton_Vy[], bool skeletonOnGround[], int skeletonDir[], Sprite skeletonSprite[], int im, int invisible_man_x[], int invisible_man_y[], float invisible_man_Vy[], bool invisible_manOnGround[], int invisible_manDir[], Sprite invisible_manSprite[], Texture& ghostTex, Texture& skeletonTex, Texture& invisible_manTex, float& player_x, float& player_y)
{
    // clear whole grid
    for (int i = 0; i < height; ++i)
        for (int j = 0; j < width; ++j)
            lvl[i][j] = ' ';

    if (level == LEVEL_1)
    {
        level1_platforms(lvl, width, height);

        //player spawn for level 1
        player_x = 530;
        player_y = 100;
    }
    else // LEVEL_2
    {
        level2_platforms(lvl, width, height);

        //simple different spawn for level 2
        player_x = 64 * 2;
        player_y = 64 * 5;
    }

    // spawn ghosts
    int i = 0;
    while (i < g)
    {
        ghostSprite[i].setTexture(ghostTex);
        ghostSprite[i].setScale(0.28f, 0.28f);

        int gx = (rand() % width) * cell_size;
        int gy = (rand() % height) * cell_size;
        int GY = gy / cell_size;
        int GX = gx / cell_size;

        if (GY < 0 || GY >= height || GX < 0 || GX >= width) continue;

        if (lvl[GY][GX] == '#')
            continue;
        if (GY + 1 < height)
            if (lvl[GY + 1][GX] != '#')
                continue;

        ghost_x[i] = gx;
        ghost_y[i] = gy;
        ghostDir[i] = (rand() % 2 == 0) ? -1 : 1;
        ghostSprite[i].setPosition(ghost_x[i], ghost_y[i]);
        ++i;
    }

    //spawn skeletons
    i = 0;
    while (i < s)
    {
        skeletonSprite[i].setTexture(skeletonTex);
        skeletonSprite[i].setScale(0.3f, 0.3f);

        int sx = (rand() % width) * cell_size;
        int sy = (rand() % height) * cell_size;
        int SY = sy / cell_size;
        int SX = sx / cell_size;

        if (SY < 0 || SY >= height || SX < 0 || SX >= width) continue;

        if (lvl[SY][SX] == '#')
            continue;
        if (SY + 1 < height)
            if (lvl[SY + 1][SX] != '#')
                continue;

        skeleton_x[i] = sx;
        skeleton_y[i] = sy;
        skeleton_Vy[i] = 0;
        skeletonOnGround[i] = false;
        skeletonDir[i] = (rand() % 2 == 0) ? -1 : 1;

        skeletonSprite[i].setPosition(skeleton_x[i], skeleton_y[i]);
        ++i;
    }

    // spawn invisible men only level 2
    if (level == LEVEL_2)
    {
        int maxInvisible = (im < 3 ? im : 3); // invisible man

        int k = 0;
        while (k < maxInvisible)
        {
            invisible_manSprite[k].setTexture(invisible_manTex);
            invisible_manSprite[k].setScale(0.8, 0.8);

            int ix = (rand() % width) * cell_size;
            int iy = (rand() % height) * cell_size;
            int IY = iy / cell_size;
            int IX = ix / cell_size;

            if (IY < 0 || IY >= height || IX < 0 || IX >= width) continue;

            if (lvl[IY][IX] == '#')
                continue;
            if (IY + 1 < height)
                if (lvl[IY + 1][IX] != '#')
                    continue;

            invisible_man_x[k] = ix;
            invisible_man_y[k] = iy;
            invisible_man_Vy[k] = 0.0f;
            invisible_manOnGround[k] = false;
            invisible_manDir[k] = (rand() % 2 == 0) ? -1 : 1;

            invisible_manSprite[k].setPosition(invisible_man_x[k], invisible_man_y[k]);
            ++k;
        }

        //any remaining invisible slots then go offscreen
        for (int t = maxInvisible; t < im; ++t)
        {
            invisible_manSprite[t].setTexture(invisible_manTex);
            invisible_manSprite[t].setScale(0.8, 0.8);
            invisible_man_x[t] = -5000;
            invisible_man_y[t] = -5000;
            invisible_man_Vy[t] = 0.0f;
            invisible_manOnGround[t] = false;
            invisible_manDir[t] = (rand() % 2 == 0) ? -1 : 1;
        }
    }
    else
    {
        // LEVEL_1 → keep invisible men off-screen
        for (int t = 0; t < im; ++t)
        {
            invisible_manSprite[t].setTexture(invisible_manTex);
            invisible_manSprite[t].setScale(0.3f, 0.3f);
            invisible_man_x[t] = -5000;
            invisible_man_y[t] = -5000;
            invisible_man_Vy[t] = 0.0f;
            invisible_manOnGround[t] = false;
            invisible_manDir[t] = (rand() % 2 == 0) ? -1 : 1;
        }
    }
}



void ghost_movement(char** lvl, int& ghost_x, int& ghost_y, int& dirX, const int cell_size, int& Gheight, int& Gwidth, int height, int width)
{
    int leftX = ghost_x / cell_size;
    int rightX = (ghost_x + Gwidth) / cell_size;
    int midX = (ghost_x + Gwidth/2) / cell_size;    
    
    //horizontal movement
    int Gspeed = 3;
    int nextX = ghost_x + dirX * Gspeed;

    int leftCol = nextX / cell_size;
    int rightCol = (nextX + Gwidth - 1) / cell_size;
    int topRow = ghost_y / cell_size;
    int bottomRow = (ghost_y + Gheight - 1) / cell_size;

    leftCol = (leftCol < 0)? 0 : (leftCol >= width)? width-1 : leftCol;
    rightCol = (rightCol < 0)? 0 : (rightCol >= width)? width-1 : rightCol; 
    topRow =  (topRow < 0)? 0 : (topRow >= height)? height-1 : topRow;
    bottomRow =  (bottomRow < 0)? 0 : (bottomRow >= height)? height-1 : bottomRow;    
    
    int facingCol = (dirX < 0) ? leftCol : rightCol;   // Which column is the ghost facing?
    bool wall = (lvl[topRow][facingCol] == '#' || lvl[bottomRow][facingCol] == '#');
    
    int belowRow = bottomRow+1;
    bool edge = false;
    if (belowRow >= height)
        edge = true;
    else if (lvl[belowRow][facingCol] != '#')
        edge = true;    
    if (wall || edge)
        dirX *= -1;     
    else
       ghost_x = nextX;
        
    //BOUNDS CHECK
    if (ghost_x < 0)
     {  ghost_x = 0;
        dirX = 1;  }
    if (ghost_x > (width * cell_size - Gwidth))
     {  ghost_x = width * cell_size - Gwidth;
        dirX = -1;  }
    if (ghost_y < 0)
      ghost_y = 0;
    if (ghost_y > (height * cell_size - Gheight))
      ghost_y = height * cell_size - Gheight;    
    
}



void skeleton_movement(char** lvl, float& offset_y, float& velocityY, bool& onGround, const float& gravity, float& terminal_Velocity, int& skeleton_x, int& skeleton_y, int& dirX, const int cell_size, int& Sheight, int& Swidth, int height, int width)
{
    offset_y = skeleton_y;
    
    //apply gravity
    velocityY += gravity;
    if (velocityY >= terminal_Velocity) 
        velocityY = terminal_Velocity;

    offset_y = skeleton_y + velocityY;

    int leftX = skeleton_x / cell_size;
    int rightX = (skeleton_x + Swidth) / cell_size;
    int midX = (skeleton_x + Swidth/2) / cell_size;
    int bottomY = static_cast<int>(offset_y + Sheight) / cell_size;

    leftX = (leftX < 0)? 0 : (leftX > width-1)? width-1 : leftX;
    rightX = (rightX < 0)? 0 : (rightX > width-1)? width-1 : rightX; 
    midX = (midX < 0)? 0 : (midX > width-1)? width-1 : midX;
    bottomY = (bottomY < 0)? 0 : (bottomY > height-1)? height-1 : bottomY; 

    char bottom_left_down = lvl[bottomY][leftX];
    char bottom_mid_down = lvl[bottomY][midX];
    char bottom_right_down = lvl[bottomY][rightX];

    if (bottom_left_down == '#' || bottom_mid_down == '#' || bottom_right_down == '#')
    {   onGround = true;
        velocityY = 0;
        skeleton_y = bottomY * cell_size - Sheight;   }
    else
    {   onGround = false;
        skeleton_y = offset_y;   }

    // horizontal movement
    int Sspeed = 2;
    int nextX = skeleton_x + dirX * Sspeed;

    int leftCol = nextX / cell_size;
    int rightCol = (nextX + Swidth - 1) / cell_size;
    int topRow = skeleton_y / cell_size;
    int bottomRow = (skeleton_y + Sheight - 1) / cell_size;

    leftCol = (leftCol < 0)? 0 : (leftCol >= width)? width-1 : leftCol;
    rightCol = (rightCol < 0)? 0 : (rightCol >= width)? width-1 : rightCol; 
    topRow =  (topRow < 0)? 0 : (topRow >= height)? height-1 : topRow;
    bottomRow =  (bottomRow < 0)? 0 : (bottomRow >= height)? height-1 : bottomRow;   

    int facingCol = (dirX < 0) ? leftCol : rightCol;
    bool wall = (lvl[topRow][facingCol] == '#' || lvl[bottomRow][facingCol] == '#');

    int belowRow = bottomRow + 1;
    bool edge = false;
    if (belowRow >= height)
        edge = true;
    else if (lvl[belowRow][facingCol] != '#')
        edge = true;

    if (wall || edge)
       dirX *= -1;
    else
       skeleton_x = nextX;

    //Bounds check
    if (skeleton_x < 0)
    {   skeleton_x = 0;
        dirX = 1;   }
    else if (skeleton_x > (width*cell_size - Swidth))
    {   skeleton_x = width*cell_size - Swidth;
        dirX = -1;   }
    if (skeleton_y < 0)
        skeleton_y = 0;
    else if (skeleton_y > (height*cell_size - Sheight))
        skeleton_y = height*cell_size - Sheight;
    
    
    if (onGround)
    {
        //For current position
        int sLeftX = skeleton_x / cell_size;
        int sRightX = (skeleton_x + Swidth) / cell_size;
        int sMidX = (skeleton_x + Swidth/2) / cell_size;

        if (sLeftX < 0)          sLeftX = 0;
        if (sLeftX > width-1)    sLeftX = width-1;
        if (sRightX < 0)         sRightX = 0;
        if (sRightX > width-1)   sRightX = width-1;
        if (sMidX < 0)           sMidX = 0;
        if (sMidX > width-1)     sMidX = width-1;

        int currentBottomRow = (skeleton_y + Sheight) / cell_size;
        if (currentBottomRow < 0)        
            currentBottomRow = 0;

        if (currentBottomRow >= height) 
           currentBottomRow = height - 1;

        int r = rand() % 1000;   


        if (r < 5)
        {
          int targetRowUp = -1;
          for (int row = currentBottomRow - 1; row >= 0; --row)
          {
             char bl = lvl[row][sLeftX];
             char bm = lvl[row][sMidX];
             char br = lvl[row][sRightX];

             if (bl == '#' || bm == '#' || br == '#')
             {   targetRowUp = row;
                 break;   }
          }
 
          if (targetRowUp != -1)
          {
              int gapUp = currentBottomRow - targetRowUp - 1;
              if (gapUp == 1)
              {
                  bool doubleCeiling = false;
                  if (targetRowUp - 1 >= 0)
                  {   char bl2 = lvl[targetRowUp - 1][sLeftX];
                      char bm2 = lvl[targetRowUp - 1][sMidX];
                      char br2 = lvl[targetRowUp - 1][sRightX];
                      if (bl2 == '#' || bm2 == '#' || br2 == '#')
                          doubleCeiling = true;                      }

                  if (!doubleCeiling)
                  {   skeleton_y = targetRowUp * cell_size - Sheight;
                      velocityY = 0;
                      onGround = true;   }
              }
          }
        }    


        else if (r >= 5 && r < 11)    
        {
            int targetRow = -1;
            for(int row=currentBottomRow+1;row<height;row++)
            {
                char bl=lvl[row][sLeftX];
                char bm=lvl[row][sMidX];
                char br=lvl[row][sRightX];

                if(bl == '#'||bm=='#'||br=='#')
                {   targetRow=row;
                    break;   }
            }

            if (targetRow!=-1)
            {
                int block_Present=targetRow- currentBottomRow-1;

                if (block_Present==1)  
                {
                    bool doubleFloor=false;
                    if (targetRow+1<height)
                    {   char bl2=lvl[targetRow+1][sLeftX];
                        char bm2=lvl[targetRow+1][sMidX];
                        char br2=lvl[targetRow+1][sRightX];
                        if (bl2=='#'||bm2=='#'||br2=='#')
                            doubleFloor = true;                        }

                    if (!doubleFloor)
                    {   skeleton_y = targetRow * cell_size - Sheight;
                        velocityY = 0;
                        onGround = true;   }
             }
        }         }
  }
}



void invisible_man_movement(char** lvl, float& offset_y, float& velocityY, bool& onGround, const float& gravity, float& terminal_Velocity, int& im_x, int& im_y, int& dirX, const int cell_size, int& IMheight, int& IMwidth, int height, int width)
{
    //random teleports
    if (rand() % 1000 < 3)
    {
        while (true)   //find tile a tile is find and spawn
        {
            int col = rand() % width;
            int row = rand() % height;

            if (row + 1 < height && lvl[row][col] == ' ' && lvl[row + 1][col] == '#')
            {
                im_x = col * cell_size;
                im_y = row * cell_size;
                break;
            }
        }
    }

    // ---- GRAVITY ----
    offset_y = im_y;

    velocityY += gravity;
    if (velocityY >= terminal_Velocity)
        velocityY = terminal_Velocity;

    offset_y = im_y + velocityY;

    int leftX  = im_x / cell_size;
    int rightX = (im_x + IMwidth) / cell_size;
    int midX   = (im_x + IMwidth / 2) / cell_size;
    int bottomY = static_cast<int>(offset_y + IMheight) / cell_size;

    if (leftX < 0) leftX = 0;
    if (leftX > width - 1) leftX = width - 1;
    if (rightX < 0) rightX = 0;
    if (rightX > width - 1) rightX = width - 1;
    if (midX < 0) midX = 0;
    if (midX > width - 1) midX = 0;
    if (bottomY < 0) bottomY = 0;
    if (bottomY > height - 1) bottomY = height - 1;

    char bottom_left_down  = lvl[bottomY][leftX];
    char bottom_mid_down   = lvl[bottomY][midX];
    char bottom_right_down = lvl[bottomY][rightX];

    if (bottom_left_down == '#' || bottom_mid_down == '#' || bottom_right_down == '#')
    {
        onGround = true;
        velocityY = 0;
        im_y = bottomY * cell_size - IMheight;
    }
    else
    {
        onGround = false;
        im_y = offset_y;
    }

    // ---- HORIZONTAL MOVEMENT (ghost-like) ----
    int IMSPEED = 3;
    int nextX = im_x + dirX * IMSPEED;

    int leftCol   = nextX / cell_size;
    int rightCol  = (nextX + IMwidth - 1) / cell_size;
    int topRow    = im_y / cell_size;
    int bottomRow = (im_y + IMheight - 1) / cell_size;

    if (leftCol < 0) leftCol = 0;
    if (leftCol > width - 1) leftCol = width - 1;
    if (rightCol < 0) rightCol = 0;
    if (rightCol > width - 1) rightCol = width - 1;
    if (topRow < 0) topRow = 0;
    if (topRow > height - 1) topRow = height - 1;
    if (bottomRow < 0) bottomRow = 0;
    if (bottomRow > height - 1) bottomRow = height - 1;

    int facingCol = (dirX < 0) ? leftCol : rightCol;
    bool wall = (lvl[topRow][facingCol] == '#' || lvl[bottomRow][facingCol] == '#');

    int belowRow = bottomRow + 1;
    bool edge = false;
    if (belowRow >= height)
        edge = true;
    else if (lvl[belowRow][facingCol] != '#')
        edge = true;

    if (wall || edge)
        dirX *= -1;
    else
        im_x = nextX;

    // ---- BOUNDS CHECK ----
    if (im_x < 0)
    {
        im_x = 0;
        dirX = 1;
    }
    else if (im_x > width * cell_size - IMwidth)
    {
        im_x = width * cell_size - IMwidth;
        dirX = -1;
    }

    if (im_y < 0)
        im_y = 0;
    else if (im_y > height * cell_size - IMheight)
        im_y = height * cell_size - IMheight;
}




void vaccum_is_On(Sprite enemySprite[], bool captured[], int count, float player_x, float player_y, int Pwidth, int Pheight, int PlayerDirection_X, int PlayerDirection_Y, float VaccumRange, float VaccumPower, float VaccumRadius, char enemy_stored[], int maxCapacity, char enemy_type,float& Vaccum_Suck_X,float& Vaccum_Suck_Y)
{
      Vaccum_Suck_X = player_x+ Pwidth/2.0f;
      Vaccum_Suck_Y = player_y+ Pheight/2.0f;


      
    int counter=0;
    for (int k=0; k<maxCapacity; ++k)
    {   if(enemy_stored[k]!=0)
        ++counter;                }

    if (counter == maxCapacity)
        return;
    

    for (int i=0; i<count; ++i)
   {
       
      if (captured[i] )
         continue;
    
      float enemy_Position_x = enemySprite[i].getPosition().x;
      float enemy_Position_y = enemySprite[i].getPosition().y;

      float dist_x = Vaccum_Suck_X - enemy_Position_x;
      float dist_y= Vaccum_Suck_Y - enemy_Position_y;
      float distance = sqrt(dist_x * dist_x + dist_y * dist_y);

      if (distance>VaccumRange)
         continue;
    
      bool inFront = false;

      if (PlayerDirection_X == 1)
          {
            inFront = (enemy_Position_x >= Vaccum_Suck_X);
        }
      else if (PlayerDirection_X == -1)
         {
         
          inFront = (enemy_Position_x <= Vaccum_Suck_X);
          
         }
      else if (PlayerDirection_Y == 1)
       { 

         inFront = (enemy_Position_y >= Vaccum_Suck_Y);

        }
      else if (PlayerDirection_Y == -1)
          {
            
             inFront = (enemy_Position_y <= Vaccum_Suck_Y);

            }
      else
          inFront = false;

      if (!inFront)
          continue;

      if (distance <= VaccumRadius && inFront)
      {   

          captured[i] = true;
          for (int l = 0; l < maxCapacity; ++l)
          {
              if (enemy_stored[l]==0)
              {  enemy_stored[l]=enemy_type;
                 break;    }
          }

          enemySprite[i].setPosition(-5000.f, -5000.f); 
      }
 
      ++counter;
      if (counter==maxCapacity)
          return;

   }
}


void draw_vacuum(RenderWindow& window,float Vaccum_Suck_X, float Vaccum_Suck_Y,int PlayerDirection_X, int PlayerDirection_Y,Sprite& VaccumSprite1,Sprite& VaccumSprite2,Sprite& VaccumSprite3,Sprite& VaccumSprite4)
{
    if (PlayerDirection_X == 1)   //right
    {
        VaccumSprite1.setPosition(Vaccum_Suck_X+40.0f, Vaccum_Suck_Y-25.0f);
        window.draw(VaccumSprite1);
    }
    else if (PlayerDirection_X == -1) //left
    {
        VaccumSprite2.setPosition(Vaccum_Suck_X-140.0f, Vaccum_Suck_Y-25.0f);
        window.draw(VaccumSprite2);
    }
    else if (PlayerDirection_Y == 1)  //down
    {
        VaccumSprite3.setPosition(Vaccum_Suck_X, Vaccum_Suck_Y);
        window.draw(VaccumSprite3);
    }
    else if (PlayerDirection_Y == -1) //up
    {
        VaccumSprite4.setPosition(Vaccum_Suck_X, Vaccum_Suck_Y-100.0f);
        window.draw(VaccumSprite4);
    }
    else if (PlayerDirection_X==0 && PlayerDirection_Y==0)
    {
        return;
    }
}



void getThrowDirection(int PlayerDirection_X, int PlayerDirection_Y, int &dx, int &dy)
{
    dx = 0;
    dy = 0;

    if (PlayerDirection_X == 1)
    {
        dx = 1;
        dy = 0;
    }
    else if (PlayerDirection_X == -1)
    {
        dx = -1;
        dy = 0;
    }
    else if (PlayerDirection_Y == 1)
    {
        dx = 0;
        dy = 1;
    }
    else if (PlayerDirection_Y == -1)
    {
        dx = 0;
        dy = -1;
    }
    else
    {
        //no key pressed default is right
        dx = 1;
        dy = 0;
    }
}




void ThrowOneEnemy(char enemy_stored[], int maxCapacity, bool GhostCaptured[], const int g, bool SkeletonCaptured[], const int s, bool InvisibleCaptured[], const int im, int ghost_x[], int ghost_y[], int ghostDir[], int skeleton_x[], int skeleton_y[], int skeletonDir[], int invisible_man_x[], int invisible_man_y[], int invisible_manDir[], float player_x, float player_y, int PlayerDirection_X, int PlayerDirection_Y, bool GhostRolling[], float GhostRollDist[], bool SkeletonRolling[], float SkeletonRollDist[], bool InvisibleRolling[], float InvisibleRollDist[])
{
    int slot = -1;
    char type = 0;

    for (int i = maxCapacity - 1; i >= 0; --i)
    {
        if (enemy_stored[i] != 0)
        {
            slot = i;
            type = enemy_stored[i];
            break;
        }
    }

    if (slot == -1)
        return;

    int dx, dy;
    getThrowDirection(PlayerDirection_X, PlayerDirection_Y, dx, dy);

    int offset = 40;

    if (type == 'g')
    {
        int idx = -1;
        for (int i = g - 1; i >= 0; --i)
        {
            if (GhostCaptured[i])
            {
                idx = i;
                break;
            }
        }
        if (idx == -1)
            return;

        GhostCaptured[idx] = false;

        if (dx == 1)
        {
            ghost_x[idx] = static_cast<int>(player_x + offset);
            ghostDir[idx] = dx;
            ghost_y[idx] = static_cast<int>(player_y);
        }
        else if (dx == -1)
        {
            ghost_x[idx] = static_cast<int>(player_x - offset);
            ghostDir[idx] = dx;
            ghost_y[idx] = static_cast<int>(player_y);
        }
        else if (dy == 1)
        {
            ghost_x[idx] = static_cast<int>(player_x);
            ghost_y[idx] = static_cast<int>(player_y + offset);
        }
        else if (dy == -1)
        {
            ghost_x[idx] = static_cast<int>(player_x);
            ghost_y[idx] = static_cast<int>(player_y - offset);
        }
        if (dx == 0)
            ghostDir[idx] = 1;

        GhostRolling[idx] = true;
        GhostRollDist[idx] = 0;
    }
    else if (type == 's')
    {
        int idx = -1;
        for (int i = s - 1; i >= 0; --i)
        {
            if (SkeletonCaptured[i])
            {
                idx = i;
                break;
            }
        }
        if (idx == -1)
            return;

        SkeletonCaptured[idx] = false;

        if (dx == 1)
        {
            skeleton_x[idx] = static_cast<int>(player_x + offset);
            skeletonDir[idx] = dx;
            skeleton_y[idx] = static_cast<int>(player_y);
        }
        else if (dx == -1)
        {
            skeleton_x[idx] = static_cast<int>(player_x - offset);
            skeletonDir[idx] = dx;
            skeleton_y[idx] = static_cast<int>(player_y);
        }
        else if (dy == 1)
        {
            skeleton_x[idx] = static_cast<int>(player_x);
            skeleton_y[idx] = static_cast<int>(player_y + offset);
        }
        else if (dy == -1)
        {
            skeleton_x[idx] = static_cast<int>(player_x);
            skeleton_y[idx] = static_cast<int>(player_y - offset);
        }

        if (dx == 0)
            skeletonDir[idx] = 1;

        SkeletonRolling[idx] = true;
        SkeletonRollDist[idx] = 0;
    }
    else if (type == 'i')
    {
        int idx = -1;
        for (int i = im - 1; i >= 0; --i)
        {
            if (InvisibleCaptured[i])
            {
                idx = i;
                break;
            }
        }
        if (idx == -1)
            return;

        InvisibleCaptured[idx] = false;

        if (dx == 1)
        {
            invisible_man_x[idx] = static_cast<int>(player_x + offset);
            invisible_manDir[idx] = dx;
            invisible_man_y[idx] = static_cast<int>(player_y);
        }
        else if (dx == -1)
        {
            invisible_man_x[idx] = static_cast<int>(player_x - offset);
            invisible_manDir[idx] = dx;
            invisible_man_y[idx] = static_cast<int>(player_y);
        }
        else if (dy == 1)
        {
            invisible_man_x[idx] = static_cast<int>(player_x);
            invisible_man_y[idx] = static_cast<int>(player_y + offset);
        }
        else if (dy == -1)
        {
            invisible_man_x[idx] = static_cast<int>(player_x);
            invisible_man_y[idx] = static_cast<int>(player_y - offset);
        }

        if (dx == 0)
            invisible_manDir[idx] = 1;

        InvisibleRolling[idx] = true;
        InvisibleRollDist[idx] = 0;
    }

    enemy_stored[slot] = 0;
}



void ThrowAllEnemies(char enemy_stored[], int maxCapacity, bool GhostCaptured[], const int g, bool SkeletonCaptured[], const int s, bool InvisibleCaptured[], const int im, int ghost_x[], int ghost_y[], int ghostDir[], int skeleton_x[], int skeleton_y[], int skeletonDir[], int invisible_man_x[], int invisible_man_y[], int invisible_manDir[], float player_x, float player_y, int PlayerDirection_X, int PlayerDirection_Y, bool GhostRolling[], float GhostRollDist[], bool SkeletonRolling[], float SkeletonRollDist[], bool InvisibleRolling[], float InvisibleRollDist[])
{
    while (true)
    {
        bool any = false;

        for (int i = 0; i < maxCapacity; ++i)
        {
            if (enemy_stored[i] != 0)
            {
                any = true;
                break;
            }
        }

        if (!any)
            break;

        ThrowOneEnemy(enemy_stored, maxCapacity, GhostCaptured, g, SkeletonCaptured, s, InvisibleCaptured, im, ghost_x, ghost_y, ghostDir, skeleton_x, skeleton_y, skeletonDir, invisible_man_x, invisible_man_y, invisible_manDir, player_x, player_y, PlayerDirection_X, PlayerDirection_Y, GhostRolling, GhostRollDist, SkeletonRolling, SkeletonRollDist, InvisibleRolling, InvisibleRollDist);
    }
}


void moveRollingEnemyUpOnePlatform(char** lvl, int width, int height, int cell_size, int &ex, int &ey, int eW, int eH)
{
    int leftX  = ex / cell_size;
    int rightX = (ex + eW - 1) / cell_size;
    int midX   = (ex + eW / 2) / cell_size;

    if (leftX < 0) leftX = 0;
    if (leftX > width - 1) leftX = width - 1;
    if (rightX < 0) rightX = 0;
    if (rightX > width - 1) rightX = width - 1;
    if (midX < 0) midX = 0;
    if (midX > width - 1) midX = width - 1;

    //current top row of the enemy
    int topRow = ey / cell_size;
    if (topRow <= 1) return;   

    int targetRow = -1;

    // look for a platform above leave at least 1 row gap
    for (int row = topRow - 2; row >= 0; --row)
    {
        char bl = lvl[row][leftX];
        char bm = lvl[row][midX];
        char br = lvl[row][rightX];

        if (bl == '#' || bm == '#' || br == '#')
        {
            targetRow = row;
            break;
        }
    }

    if (targetRow == -1)
        return;

    // place enemy standing on top of that platform
    ey = targetRow * cell_size - eH;
}





void moveRollingEnemyDownOnePlatform(char** lvl, int width, int height, int cell_size,int &ex, int &ey, int eW, int eH)
{
    int leftX  = ex / cell_size;
    int rightX = (ex + eW - 1) / cell_size;
    int midX   = (ex + eW / 2) / cell_size;

    if (leftX < 0) leftX = 0;
    if (leftX > width-1) leftX = width-1;
    if (rightX < 0) rightX = 0;
    if (rightX > width-1) rightX = width-1;
    if (midX < 0) midX = 0;
    if (midX > width-1) midX = width-1;

    int bottomRow = (ey + eH - 1) / cell_size;
    if (bottomRow < 0) bottomRow = 0;
    if (bottomRow > height-1) bottomRow = height-1;

    if (bottomRow >= height - 2)
        return;

    int targetRow = -1;
    for (int row = bottomRow + 2; row < height; ++row)
    {
        char bl = lvl[row][leftX];
        char bm = lvl[row][midX];
        char br = lvl[row][rightX];

        if (bl == '#' || bm == '#' || br == '#')
        {
            targetRow = row;
            break;
        }
    }

    if (targetRow == -1)
        return;

    ey = targetRow * cell_size - eH;
}



void updateRollingEnemies(char** lvl, int width, int height, int cell_size, int ghost_x[], int ghost_y[], int ghostDir[], bool GhostCaptured[], bool GhostRolling[], float GhostRollDist[], int g, int skeleton_x[], int skeleton_y[], int skeletonDir[], bool SkeletonCaptured[], bool SkeletonRolling[], float SkeletonRollDist[], int s, int invisible_man_x[], int invisible_man_y[], int invisible_manDir[], bool InvisibleCaptured[], bool InvisibleRolling[], float InvisibleRollDist[], int im, int Gwidth, int Gheight, int Swidth, int Sheight, int IMwidth, int IMheight, bool W_T_edge, bool S_T_edge, Sprite ghostSprite[], Sprite skeletonSprite[], Sprite invisible_manSprite[])
{
    const int ROLL_SPEED = 12;
    const int FALL_SPEED = 10; 

    for (int i = 0; i < g; i++)
    {
        if (GhostRolling[i])
        {
            
            if (GhostRollDist[i] == 0)
            {
                if (W_T_edge)
                    moveRollingEnemyUpOnePlatform(lvl, width, height, cell_size, ghost_x[i], ghost_y[i], Gwidth, Gheight);
                else if (S_T_edge)
                    moveRollingEnemyDownOnePlatform(lvl, width, height, cell_size, ghost_x[i], ghost_y[i], Gwidth, Gheight);
            }

            int nextX = ghost_x[i] + (ghostDir[i] * ROLL_SPEED);
            
            int leftTile = nextX / cell_size;
            int rightTile = (nextX + Gwidth) / cell_size;
            int topTile = ghost_y[i] / cell_size;
            int bottomTile = (ghost_y[i] + Gheight - 1) / cell_size;

            if (leftTile < 0) leftTile = 0;
            if (rightTile >= width) rightTile = width - 1;
            
            bool hitWall = false;
            if (ghostDir[i] == 1)
            {
                if (rightTile < width && (lvl[topTile][rightTile] == '#' || lvl[bottomTile][rightTile] == '#'))
                    hitWall = true;
            }
            else
            {
                if (leftTile >= 0 && (lvl[topTile][leftTile] == '#' || lvl[bottomTile][leftTile] == '#'))
                    hitWall = true;
            }

            if (hitWall)
            {
                ghostDir[i] *= -1;
            }
            else
            {
                ghost_x[i] = nextX;
            }

            int centerX = (ghost_x[i] + Gwidth / 2) / cell_size;
            int belowY = (ghost_y[i] + Gheight) / cell_size;

            if (belowY < height && belowY >= 0 && centerX >= 0 && centerX < width)
            {
                if (lvl[belowY][centerX] != '#')
                {
                    ghost_y[i] += FALL_SPEED;
                }
              
            }
            
            GhostRollDist[i] += ROLL_SPEED;

            if (GhostRollDist[i] > 15 * cell_size)
            {
                GhostRolling[i] = false;
                GhostCaptured[i] = true; 
                ghost_x[i] = -5000;
                ghost_y[i] = -5000;
            }

            for (int j = 0; j < g; j++)
            {
                if (i == j) 
                    continue; 
                if (!GhostCaptured[j] && !GhostRolling[j])
                {
                    if (abs(ghost_x[i] - ghost_x[j]) < 50 && abs(ghost_y[i] - ghost_y[j]) < 50)
                    {
                        GhostCaptured[j] = true;
                        ghost_x[j] = -5000;
                        ghost_y[j] = -5000;
                    }
                }
            }
            for (int k = 0; k < s; k++)
            {
                if (!SkeletonCaptured[k] && !SkeletonRolling[k])
                {
                    if (abs(ghost_x[i] - skeleton_x[k]) < 50 && abs(ghost_y[i] - skeleton_y[k]) < 50)
                    {
                        SkeletonCaptured[k] = true;
                        skeleton_x[k] = -5000;
                        skeleton_y[k] = -5000;
                    }
                }
            }
            for (int m = 0; m < im; m++)
            {
                if (!InvisibleCaptured[m] && !InvisibleRolling[m])
                {
                     if (abs(ghost_x[i] - invisible_man_x[m]) < 50 && abs(ghost_y[i] - invisible_man_y[m]) < 50)
                     {
                         InvisibleCaptured[m] = true;
                         invisible_man_x[m] = -5000;
                         invisible_man_y[m] = -5000;
                     }
                }
            }

            ghostSprite[i].setPosition(ghost_x[i], ghost_y[i]);
        }
    }

    for (int i = 0; i < s; i++)
    {
        if (SkeletonRolling[i])
        {
            if (SkeletonRollDist[i] == 0)
            {
                if (W_T_edge)
                    moveRollingEnemyUpOnePlatform(lvl, width, height, cell_size, skeleton_x[i], skeleton_y[i], Swidth, Sheight);
                else if (S_T_edge)
                    moveRollingEnemyDownOnePlatform(lvl, width, height, cell_size, skeleton_x[i], skeleton_y[i], Swidth, Sheight);
            }

            int nextX = skeleton_x[i] + (skeletonDir[i] * ROLL_SPEED);
            int leftTile = nextX / cell_size;
            int rightTile = (nextX + Swidth) / cell_size;
            int topTile = skeleton_y[i] / cell_size;
            int bottomTile = (skeleton_y[i] + Sheight - 1) / cell_size;

            if (leftTile < 0) leftTile = 0;
            if (rightTile >= width) rightTile = width - 1;

            bool hitWall = false;
            if (skeletonDir[i] == 1) {
                if (rightTile < width && (lvl[topTile][rightTile] == '#' || lvl[bottomTile][rightTile] == '#')) hitWall = true;
            } else {
                if (leftTile >= 0 && (lvl[topTile][leftTile] == '#' || lvl[bottomTile][leftTile] == '#')) hitWall = true;
            }

            if (hitWall) skeletonDir[i] *= -1;
            else skeleton_x[i] = nextX;

            int centerX = (skeleton_x[i] + Swidth / 2) / cell_size;
            int belowY = (skeleton_y[i] + Sheight) / cell_size;

            if (belowY < height && belowY >= 0 && centerX >= 0 && centerX < width) {
                if (lvl[belowY][centerX] != '#') skeleton_y[i] += FALL_SPEED;
            }

            SkeletonRollDist[i] += ROLL_SPEED;
            if (SkeletonRollDist[i] > 15 * cell_size) {
                SkeletonRolling[i] = false;
                SkeletonCaptured[i] = true; 
                skeleton_x[i] = -5000;      
                skeleton_y[i] = -5000;
            }

            
            
            for (int j = 0; j < g; j++)
            {
                if (!GhostCaptured[j] && !GhostRolling[j])
                {
                    if (abs(skeleton_x[i] - ghost_x[j]) < 50 && abs(skeleton_y[i] - ghost_y[j]) < 50)
                    {
                        GhostCaptured[j] = true;
                        ghost_x[j] = -5000;
                        ghost_y[j] = -5000;
                    }
                }
            }
            for (int k = 0; k < s; k++)
            {
                if (i == k) 
                    continue; 
                if (!SkeletonCaptured[k] && !SkeletonRolling[k])
                {
                    if (abs(skeleton_x[i] - skeleton_x[k]) < 50 && abs(skeleton_y[i] - skeleton_y[k]) < 50)
                    {
                        SkeletonCaptured[k] = true;
                        skeleton_x[k] = -5000;
                        skeleton_y[k] = -5000;
                    }
                }
            }
            for (int m = 0; m < im; m++)
            {
               if (!InvisibleCaptured[m] && !InvisibleRolling[m])
               {
                      if (abs(skeleton_x[i] - invisible_man_x[m]) < 50 && abs(skeleton_y[i] - invisible_man_y[m]) < 50)
                        {
                              InvisibleCaptured[m] = true;
                              invisible_man_x[m] = -5000;
                              invisible_man_y[m] = -5000;
                        }
                }
            }

          skeletonSprite[i].setPosition(skeleton_x[i], skeleton_y[i]);
        }
    }
    
    for (int i = 0; i < im; i++)
{
    if (InvisibleRolling[i])
    {
        if (InvisibleRollDist[i] == 0)
        {
            if (W_T_edge)
                moveRollingEnemyUpOnePlatform(lvl, width, height, cell_size, invisible_man_x[i], invisible_man_y[i], IMwidth, IMheight);
            else if (S_T_edge)
                moveRollingEnemyDownOnePlatform(lvl, width, height, cell_size, invisible_man_x[i], invisible_man_y[i], IMwidth, IMheight);
        }

        int nextX = invisible_man_x[i] + (invisible_manDir[i] * ROLL_SPEED);
        int leftTile   = nextX / cell_size;
        int rightTile  = (nextX + IMwidth) / cell_size;
        int topTile    = invisible_man_y[i] / cell_size;
        int bottomTile = (invisible_man_y[i] + IMheight - 1) / cell_size;

        if (leftTile < 0) leftTile = 0;
        if (rightTile >= width) rightTile = width - 1;
        if (topTile < 0) topTile = 0;
        if (bottomTile >= height) bottomTile = height - 1;

        bool hitWall = false;
        if (invisible_manDir[i] == 1)
        {
            if (rightTile < width && (lvl[topTile][rightTile] == '#' || lvl[bottomTile][rightTile] == '#'))
                hitWall = true;
        }
        else
        {
            if (leftTile >= 0 && (lvl[topTile][leftTile] == '#' || lvl[bottomTile][leftTile] == '#'))
                hitWall = true;
        }

        if (hitWall)
            invisible_manDir[i] *= -1;
        else
            invisible_man_x[i] = nextX;

        int centerX = (invisible_man_x[i] + IMwidth / 2) / cell_size;
        int belowY  = (invisible_man_y[i] + IMheight) / cell_size;

        if (belowY < height && belowY >= 0 && centerX >= 0 && centerX < width)
        {
            if (lvl[belowY][centerX] != '#')
                invisible_man_y[i] += FALL_SPEED;
        }

        InvisibleRollDist[i] += ROLL_SPEED;

        if (InvisibleRollDist[i] > 15 * cell_size)
        {
            InvisibleRolling[i] = false;
            InvisibleCaptured[i] = true;
            invisible_man_x[i] = -5000;
            invisible_man_y[i] = -5000;
        }

        // hit ghosts
        for (int j = 0; j < g; j++)
        {
            if (!GhostCaptured[j] && !GhostRolling[j])
            {
                if (abs(invisible_man_x[i] - ghost_x[j]) < 50 && abs(invisible_man_y[i] - ghost_y[j]) < 50)
                {
                    GhostCaptured[j] = true;
                    ghost_x[j] = -5000;
                    ghost_y[j] = -5000;
                }
            }
        }
        // hit skeletons
        for (int k = 0; k < s; k++)
        {
            if (!SkeletonCaptured[k] && !SkeletonRolling[k])
            {
                if (abs(invisible_man_x[i] - skeleton_x[k]) < 50 && abs(invisible_man_y[i] - skeleton_y[k]) < 50)
                {
                    SkeletonCaptured[k] = true;
                    skeleton_x[k] = -5000;
                    skeleton_y[k] = -5000;
                }
            }
        }
        // hit other invisible men
        for (int m = 0; m < im; m++)
        {
            if (i == m) continue;
            if (!InvisibleCaptured[m] && !InvisibleRolling[m])
            {
                if (abs(invisible_man_x[i] - invisible_man_x[m]) < 50 && abs(invisible_man_y[i] - invisible_man_y[m]) < 50)
                {
                    InvisibleCaptured[m] = true;
                    invisible_man_x[m] = -5000;
                    invisible_man_y[m] = -5000;
                }
            }
        }

        invisible_manSprite[i].setPosition(invisible_man_x[i], invisible_man_y[i]);
    }
}




}

void collision(RenderWindow& window, float& player_x, float& player_y, int g, int ghost_x[], int ghost_y[], bool GhostCaptured[], bool GhostRolling[], int s, int skeleton_x[], int skeleton_y[], bool SkeletonCaptured[], bool SkeletonRolling[], int im, int invisible_man_x[], int invisible_man_y[], bool InvisibleCaptured[], bool InvisibleRolling[], Sprite& PlayerSprite, int& lives)
{
    static bool wasColliding = false;
    bool isColliding = false;

    for (int i = 0; i < g; i++)
    {
        if ((player_y > ghost_y[i] - 50 && player_y < ghost_y[i] + 50) && (player_x > ghost_x[i] - 50 && player_x < ghost_x[i] + 50))
        {
            if (GhostCaptured[i] || GhostRolling[i])
                continue;
            else
            {
                isColliding = true;
                break;
            }
        }
    }

    if (!isColliding)
    {
        for (int i = 0; i < s; i++)
        {
            if ((player_y > skeleton_y[i] - 50 && player_y < skeleton_y[i] + 50) && (player_x > skeleton_x[i] - 50 && player_x < skeleton_x[i] + 50))
            {
                if (SkeletonCaptured[i] || SkeletonRolling[i])
                    continue;
                else
                {
                    isColliding = true;
                    break;
                }
            }
        }
    }

    if (!isColliding)
    {
        for (int i = 0; i < im; ++i)
        {
            if ((player_y > invisible_man_y[i] - 50 && player_y < invisible_man_y[i] + 50) &&
                (player_x > invisible_man_x[i] - 50 && player_x < invisible_man_x[i] + 50))
            {
                if (InvisibleCaptured[i] || InvisibleRolling[i])
                    continue;
                else
                {
                    isColliding = true;
                    break;
                }
            }
        }
    }


    if (isColliding && !wasColliding)
    {
        lives--;
        player_x = 530;
        player_y = 100;   
        PlayerSprite.setPosition(player_x, player_y);
    }

    wasColliding = isColliding;
}

void powerup (RenderWindow& window, char** lvl, int height, int width, int cell_size, float player_x, float player_y, int Pwidth, int Pheight, float& speed, float& VaccumRange, float& VaccumRadius, int& lives)
{
    static bool prev_speed = false;
    static int p1x, p1y;
    static Texture speedTexture;
    static Sprite speedSprite;
    static bool speedLoaded = false;

    if (!speedLoaded)
    {
        speedTexture.loadFromFile("Asset/Sprites/speed.png");
        speedSprite.setTexture(speedTexture);
        speedSprite.setScale(0.4, 0.4);
        speedLoaded = true;
    }

    if (!prev_speed)
    {    
         int r1 = rand() % 1000;
         if (r1 == 1)
         {
             while (true)
             {
                p1x = (rand() % width) * cell_size;
                p1y = (rand() % height) * cell_size;
                int P1Y = p1y/cell_size;
                int P1X = p1x/cell_size;
                if (P1Y+1 < height && lvl[P1Y][P1X] != '#' && lvl[P1Y+1][P1X] == '#')
                  {  prev_speed = true;  
                     break;              }  
             }
         }
    }                      

    if (prev_speed)
    { 
        speedSprite.setPosition(p1x, p1y);
        window.draw(speedSprite);
        if (player_x + Pwidth > p1x && player_x < p1x + cell_size && player_y + Pheight > p1y && player_y < p1y + cell_size)
        {
            speed *= 2;
            prev_speed = false;
        }
    }
    
    
    static bool prev_range = false;
    static int p2x, p2y;
    static Texture rangeTexture;
    static Sprite rangeSprite;
    static bool rangeLoaded = false;

    if (!rangeLoaded)
    {
        rangeTexture.loadFromFile("Asset/Sprites/range.png");
        rangeSprite.setTexture(rangeTexture);
        rangeSprite.setScale(0.4, 0.4);
        rangeLoaded = true;
    }

    if (!prev_range)
    {    
         int r2 = rand() % 1000;
         if (r2 == 2)
         {
             while (true)
             {
                p2x = (rand() % width) * cell_size;
                p2y = (rand() % height) * cell_size;
                int P2Y = p2y/cell_size;
                int P2X = p2x/cell_size;
                if (P2Y+1 < height && lvl[P2Y][P2X] != '#' && lvl[P2Y+1][P2X] == '#')
                  {  prev_range = true;  
                     break;              }  
             }
         }
    } 

    if (prev_range)
    { 
        rangeSprite.setPosition(p2x, p2y);
        window.draw(rangeSprite);
        if (player_x + Pwidth > p2x && player_x < p2x + cell_size && player_y + Pheight > p2y && player_y < p2y + cell_size)
        {
            VaccumRange *= 2;
            prev_range = false;
        }
    }

   
    static bool prev_power = false;
    static int p3x, p3y;
    static Texture powerTexture;
    static Sprite powerSprite;
    static bool powerLoaded = false;

    if (!powerLoaded)
    {
        powerTexture.loadFromFile("Asset/Sprites/power.png");
        powerSprite.setTexture(powerTexture);
        powerSprite.setScale(0.4, 0.4);
        powerLoaded = true;
    }

    if (!prev_power)
    {    
         int r3 = rand() % 1000;
         if (r3 == 3)
         {
             while (true)
             {
                p3x = (rand() % width) * cell_size;
                p3y = (rand() % height) * cell_size;
                int P3Y = p3y/cell_size;
                int P3X = p3x/cell_size;
                if (P3Y+1 < height && lvl[P3Y][P3X] != '#' && lvl[P3Y+1][P3X] == '#')
                  {  prev_power = true;  
                     break;              }  
             }
         }
    } 

    if (prev_power)
    { 
        powerSprite.setPosition(p3x, p3y);
        window.draw(powerSprite);
        if (player_x + Pwidth > p3x && player_x < p3x + cell_size && player_y + Pheight > p3y && player_y < p3y + cell_size)
        {
            VaccumRadius *= 2;
            prev_power = false;
        }
    }


    static bool prev_extra_life = false;
    static int p4x, p4y;
    static Texture extra_lifeTexture;
    static Sprite extra_lifeSprite;
    static bool extra_lifeLoaded = false;

    if (!extra_lifeLoaded)
    {
        extra_lifeTexture.loadFromFile("Asset/Sprites/extra_life.png");
        extra_lifeSprite.setTexture(extra_lifeTexture);
        extra_lifeSprite.setScale(0.4, 0.4);
        extra_lifeLoaded = true;
    }

    if (!prev_extra_life)
    {    
         int r4 = rand() % 1000;
         if (r4 == 4)
         {
             while (true)
             {
                p4x = (rand() % width) * cell_size;
                p4y = (rand() % height) * cell_size;
                int P4Y = p4y/cell_size;
                int P4X = p4x/cell_size;
                if (P4Y+1 < height && lvl[P4Y][P4X] != '#' && lvl[P4Y+1][P4X] == '#')
                  {  prev_extra_life = true;  
                     break;              }  
             }
         }
    }    

    if (prev_extra_life)
    { 
        extra_lifeSprite.setPosition(p4x, p4y);
        window.draw(extra_lifeSprite);
        if (player_x + Pwidth > p4x && player_x < p4x + cell_size && player_y + Pheight > p4y && player_y < p4y + cell_size)
        {
            lives++;
            prev_extra_life = false;
        }
    }    
}

bool isSolid(char c)
{
    return (c == '#' || c == '/');
}

bool findSlopeTile(char** lvl, int height, int width,
                   float player_x, int Pwidth, int cell_size,
                   int rowStart, int rowEnd,
                   int& outRow, int& outCol)
{
    if (rowStart < 0) rowStart = 0;
    if (rowEnd   > height - 1) rowEnd = height - 1;

    int leftX  = static_cast<int>(player_x) / cell_size;
    int rightX = static_cast<int>(player_x + Pwidth) / cell_size;

    if (leftX < 0)         leftX = 0;
    if (rightX > width-1)  rightX = width - 1;

    for (int r = rowEnd; r >= rowStart; --r)
    {
        for (int c = leftX; c <= rightX; ++c)
        {
            if (lvl[r][c] == '/')
            {
                outRow = r;
                outCol = c;
                return true;
            }
        }
    }
    return false;
}

void addRandomPlatformChunks(char** lvl, int width, int height)
{
    int numChunks = 6;
    int midCol = width / 2;

    for (int n = 0; n < numChunks; ++n)
    {
        bool placed = false;
        int attempts = 0;

        while (!placed && attempts < 40)
        {
            attempts++;

            int length = 3 + (rand() % 3);

            int row = 1 + (rand() % (height - 2));

            bool spawnRightSide = (rand() % 2 == 0);

            int startMin, startMax;

            if (spawnRightSide)
            {
                startMin = max(1, midCol);
                startMax = (width - 1) - length;
            }
            else
            {
                startMin = 1;
                startMax = max(1, midCol - length);
            }

            if (startMax < startMin)
            {
                startMin = 1;
                startMax = (width - 1) - length;
            }

            if (startMax < startMin)
                break;

            int colRange = startMax - startMin + 1;
            int col = startMin + (rand() % colRange);

            bool intersectsSlopeZone = false;
            for (int i = 0; i < length; ++i)
            {
                int cc = col + i;
                if (row >= 1 && row <= 9 && cc >= 4 && cc <= 12)
                {
                    intersectsSlopeZone = true;
                    break;
                }
            }
            if (intersectsSlopeZone)
                continue;

            bool canPlace = true;
            for (int i = 0; i < length; ++i)
            {
                char tile = lvl[row][col + i];
                if (tile != ' ')
                {
                    canPlace = false;
                    break;
                }
            }

            if (!canPlace)
                continue;

            bool aboveBelowOk = true;
            for (int i = 0; i < length && aboveBelowOk; ++i)
            {
                int cc = col + i;
                if (row - 1 >= 0 && lvl[row - 1][cc] == '#')
                    aboveBelowOk = false;
                if (row + 1 < height && lvl[row + 1][cc] == '#')
                    aboveBelowOk = false;
            }

            if (!aboveBelowOk)
                continue;

            bool hasTripleBefore = false;
            int run = 0;
            for (int j = 0; j < width; ++j)
            {
                char t = lvl[row][j];
                if (t == '#')
                {
                    run++;
                    if (run >= 3)
                    {
                        hasTripleBefore = true;
                        break;
                    }
                }
                else
                {
                    run = 0;
                }
            }

            bool hasTripleAfter = false;
            run = 0;
            for (int j = 0; j < width; ++j)
            {
                char t = lvl[row][j];

                if (j >= col && j < col + length)
                    t = '#';

                if (t == '#')
                {
                    run++;
                    if (run >= 3)
                    {
                        hasTripleAfter = true;
                        break;
                    }
                }
                else
                {
                    run = 0;
                }
            }

            if (hasTripleAfter && !hasTripleBefore)
                continue;

            for (int i = 0; i < length; ++i)
            {
                lvl[row][col + i] = '#';
            }

            placed = true;
        }
    }
}