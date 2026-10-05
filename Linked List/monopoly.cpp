
// for GUI
#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

// Nicolas Holmgren
// CS 210-11

// Part 2: Monopoly Game
// Use your linked list to build a circular board with at least 10 properties.

// Each property stores its name, cost, owner, and next pointer.

// Players move around the board, wrapping back to the beginning.

// Players can buy unowned properties, but cannot buy owned properties.

// Simulate at least 10 turns and print the results

// BONUS POINTS IF YOU CREATE A GUI



// We will be building a monopoly game with multiple players
// there will be multiple properties which will be nodes
// the user can buy any property that is unowned

// Once a player has bought a property we will need to update the node
// with the player that owns that property (it cant be bought by anyone else it is now locked)

// Each time a player lands on a property that is owned by another player they will have to pay the rent

// We also need to assign to each property the rental cost

struct Property {
    // name of property
    string name;
    // cost to buy property
    int cost;
    // players name if owned, else in owned
    string owner;
    // price non own player will have to pay when landing on it
    int rentalPrice;
    // pointer to next node
    Property* next;
};


struct Player {
    string name;
    int money;

    Property* currentPosition;
};

// What the GUI needs to know about the game after each turn
struct GameState {
    // last dice rolled (0 before anyone has rolled)
    int die1 = 0;
    int die2 = 0;
    // property the last player landed on (highlighted on the board)
    Property* lastLanded = nullptr;
    // one message per turn, newest at the back
    vector<string> log;
};

int rollDice() {
    // rand() % 6 generates 0 through 5. Adding 1 makes it 1 through 6.
    return (rand() % 6) + 1;
}


void append(Property*& head, string name, int cost, int rentalPrice) {
    Property* newProperty = new Property{name, cost, "None", rentalPrice, nullptr};


    if (head == nullptr) {
        head = newProperty;
        head->next = head;

        return;
    }

    Property* current = head;

    while (current->next != head) {
        current = current->next;
    }


    current->next = newProperty;
    newProperty->next = head;
}

// populate the board
void createBoard (Property*& head) {

    // I created the first few appends, then had AI generate the rest to streamline the properties on the board
    append(head, "SDSU Transit Center (GO)", 0, 0);

    // 2. The Early Properties (Lower cost/rent)
    append(head, "Adams Humanities", 20, 2);
    append(head, "Art North", 30, 5);
    append(head, "Lamden Hall", 35, 7);
    append(head, "Scripps Cottage", 60, 10);
    append(head, "Turtle Pond", 70, 12);

    // 3. The Mid-Board Properties
    append(head, "Calpulli Center", 100, 15);
    append(head, "Love Library", 120, 20);
    append(head, "Viejas Arena", 160, 25);
    append(head, "Aztec Recreation Center (ARC)", 180, 30);
    append(head, "Peterson Gym", 200, 35);

    // 4. The Premium STEM Corner
    append(head, "EIS Complex", 250, 50);
    append(head, "GMCS Building", 280, 65);
    append(head, "Hepner Hall", 310, 80);

    // 5. The Top-Tier Properties (Your original endpoints)
    append(head, "Storm Hall", 340, 175);
    append(head, "Aztec Student Union", 450, 240);
}

void takeTurn(Player& current, Player& opponent, GameState& state) {
    int die1 = rollDice();
    int die2 = rollDice();
    int totalSteps = die1 + die2;

    state.die1 = die1;
    state.die2 = die2;

    // builds the one-line summary shown in the GUI activity log
    ostringstream summary;
    summary << current.name << " rolled " << totalSteps << " (" << die1 << "+" << die2 << "). ";

    cout << current.name << " rolled a " << totalSteps << " and moves forward... " << endl;

    for (int i = 0; i < totalSteps; i++) {
        current.currentPosition = current.currentPosition->next;
    }


    Property* landedProperty = current.currentPosition;
    state.lastLanded = landedProperty;
    cout << current.name << " landed on " << landedProperty->name << endl;
    summary << "Landed on " << landedProperty->name << ". ";



    if (landedProperty->owner == "None" && landedProperty->cost > 0) {
        if (current.money >= landedProperty->cost) {
            current.money -= landedProperty->cost;
            landedProperty->owner = current.name;

            cout << " -> bought " << landedProperty->name << " for $" << landedProperty->cost << "." << endl;
            summary << "Bought for $" << landedProperty->cost << "!";
        } else {
            cout << " not enough money to buy " << landedProperty->name << endl;
            summary << "Not enough money to buy it.";
        }
    }

    else if (landedProperty->owner == opponent.name) {
        current.money -= landedProperty->rentalPrice;

        opponent.money += landedProperty->rentalPrice;

        cout << " Paid $" << landedProperty->rentalPrice << " in rent to " <<opponent.name << endl;
        summary << "Paid $" << landedProperty->rentalPrice << " rent to " << opponent.name << ".";
    } else {
        cout << " just visiting or resting on owned property." << endl;
        summary << "Just visiting.";
    }

    cout << " Remaining balance: $" << current.money << endl;

    state.log.push_back(summary.str());
}

// counts how many properties on the board a player owns
int countOwned(Property* head, const string& playerName) {
    int count = 0;
    Property* current = head;
    do {
        if (current->owner == playerName) {
            count++;
        }
        current = current->next;
    } while (current != head);
    return count;
}

// ---------------------------------------------------------------------------
// GUI
// ---------------------------------------------------------------------------

const unsigned int WINDOW_W = 1100;
const unsigned int WINDOW_H = 720;

// the board is a 5x5 square of tiles; its outer ring holds exactly 16 tiles
const float BOARD_X = 20.f;
const float BOARD_Y = 20.f;
const float TILE = 136.f;

// right-hand side panel (header, player cards, activity log)
const float PANEL_X = 720.f;
const float PANEL_W = 360.f;

const sf::Color BG(30, 30, 30);
const sf::Color TILE_BG(48, 48, 52);
const sf::Color CENTER_BG(38, 38, 41);
const sf::Color GRID_LINE(20, 20, 20);
const sf::Color TEXT_MAIN(235, 235, 235);
const sf::Color TEXT_DIM(150, 150, 155);
const sf::Color GOLD(255, 196, 0);
const sf::Color SDSU_RED(196, 30, 58);
const sf::Color P1_COLOR(230, 70, 70);
const sf::Color P2_COLOR(70, 140, 235);

// Monopoly-style color group for each spot on the board
sf::Color groupColor(int index) {
    if (index == 0)  return SDSU_RED;                 // GO
    if (index <= 2)  return sf::Color(140, 85, 55);   // brown
    if (index <= 5)  return sf::Color(150, 205, 235); // light blue
    if (index <= 8)  return sf::Color(215, 60, 150);  // pink
    if (index <= 10) return sf::Color(240, 145, 40);  // orange
    if (index <= 13) return sf::Color(60, 170, 90);   // green (STEM corner)
    return sf::Color(40, 80, 190);                    // dark blue (top tier)
}

// top-left corner of tile `index`, laid out like a real Monopoly board:
// GO in the bottom-right, then left along the bottom, up the left side,
// right along the top, and down the right side back to GO
sf::Vector2f tilePosition(int index) {
    int col, row;
    if (index <= 4)       { col = 4 - index;   row = 4; }
    else if (index <= 8)  { col = 0;           row = 8 - index; }
    else if (index <= 12) { col = index - 8;   row = 0; }
    else                  { col = 4;           row = index - 12; }
    return sf::Vector2f(BOARD_X + col * TILE, BOARD_Y + row * TILE);
}

// position of a property in the board vector (-1 if not found)
int indexOf(const vector<Property*>& tiles, Property* property) {
    for (int i = 0; i < (int)tiles.size(); i++) {
        if (tiles[i] == property) {
            return i;
        }
    }
    return -1;
}

sf::Color playerColor(const string& name, const Player& player1) {
    return name == player1.name ? P1_COLOR : P2_COLOR;
}

// splits a message into lines that fit inside maxWidth pixels
vector<string> wrapText(const string& message, const sf::Font& font, unsigned int size, float maxWidth,
                        bool bold = false) {
    vector<string> lines;
    istringstream words(message);
    string word;
    string line;
    sf::Text measure("", font, size);
    if (bold) {
        measure.setStyle(sf::Text::Bold);
    }

    while (words >> word) {
        string candidate = line.empty() ? word : line + " " + word;
        measure.setString(candidate);
        if (measure.getLocalBounds().width > maxWidth && !line.empty()) {
            lines.push_back(line);
            line = word;
        } else {
            line = candidate;
        }
    }
    if (!line.empty()) {
        lines.push_back(line);
    }
    return lines;
}

// draws text with its top-left at (x, y)
void drawText(sf::RenderWindow& window, const sf::Font& font, const string& str,
              unsigned int size, float x, float y, sf::Color color, bool bold = false) {
    sf::Text text(str, font, size);
    text.setFillColor(color);
    if (bold) {
        text.setStyle(sf::Text::Bold);
    }
    text.setPosition(x, y);
    window.draw(text);
}

// draws text horizontally centered on centerX
void drawCentered(sf::RenderWindow& window, const sf::Font& font, const string& str,
                  unsigned int size, float centerX, float y, sf::Color color, bool bold = false) {
    sf::Text text(str, font, size);
    text.setFillColor(color);
    if (bold) {
        text.setStyle(sf::Text::Bold);
    }
    sf::FloatRect bounds = text.getLocalBounds();
    text.setPosition(centerX - bounds.width / 2.f - bounds.left, y);
    window.draw(text);
}

void drawRect(sf::RenderWindow& window, float x, float y, float width, float height,
              sf::Color fill, sf::Color outline = sf::Color::Transparent, float thickness = 0.f) {
    sf::RectangleShape rect(sf::Vector2f(width, height));
    rect.setPosition(x, y);
    rect.setFillColor(fill);
    rect.setOutlineThickness(thickness);
    rect.setOutlineColor(outline);
    window.draw(rect);
}

// a round player token with the player number inside
void drawToken(sf::RenderWindow& window, const sf::Font& font, float centerX, float centerY,
               sf::Color color, const string& label) {
    const float radius = 13.f;
    sf::CircleShape token(radius);
    token.setOrigin(radius, radius);
    token.setPosition(centerX, centerY);
    token.setFillColor(color);
    token.setOutlineThickness(2.f);
    token.setOutlineColor(sf::Color::White);
    window.draw(token);
    drawCentered(window, font, label, 14, centerX, centerY - 10, sf::Color::White, true);
}

// one property tile on the board
void drawTile(sf::RenderWindow& window, const sf::Font& font, Property* property, int index,
              const Player& player1, bool highlighted) {
    sf::Vector2f pos = tilePosition(index);
    float centerX = pos.x + TILE / 2.f;

    drawRect(window, pos.x, pos.y, TILE, TILE, TILE_BG, GRID_LINE, 1.f);

    if (index == 0) {
        // GO tile gets the full SDSU red treatment
        drawRect(window, pos.x, pos.y, TILE, TILE, sf::Color(120, 20, 38));
        drawCentered(window, font, "GO", 34, centerX, pos.y + 8, sf::Color::White, true);
        vector<string> lines = wrapText(property->name, font, 13, TILE - 12);
        float lineY = pos.y + 52;
        for (const string& line : lines) {
            drawCentered(window, font, line, 13, centerX, lineY, TEXT_MAIN);
            lineY += 16;
        }
    } else {
        // colored band across the top, like a Monopoly deed
        drawRect(window, pos.x, pos.y, TILE, 20, groupColor(index));

        vector<string> lines = wrapText(property->name, font, 13, TILE - 12, true);
        float lineY = pos.y + 25;
        for (const string& line : lines) {
            drawCentered(window, font, line, 13, centerX, lineY, TEXT_MAIN, true);
            lineY += 16;
        }

        drawCentered(window, font, "$" + to_string(property->cost) + "  |  rent $" + to_string(property->rentalPrice),
                     12, centerX, pos.y + 66, TEXT_DIM);

        // owner strip along the bottom
        if (property->owner != "None") {
            sf::Color ownerColor = playerColor(property->owner, player1);
            drawRect(window, pos.x, pos.y + TILE - 18, TILE, 18, ownerColor);
            drawCentered(window, font, property->owner, 12, centerX, pos.y + TILE - 17, sf::Color::White, true);
        }
    }

    if (highlighted) {
        drawRect(window, pos.x + 2, pos.y + 2, TILE - 4, TILE - 4, sf::Color::Transparent, GOLD, 3.f);
    }
}

// a die face with pips; value 0 draws a blank die
void drawDie(sf::RenderWindow& window, float x, float y, float size, int value) {
    sf::RectangleShape face(sf::Vector2f(size, size));
    face.setPosition(x, y);
    face.setFillColor(sf::Color(245, 245, 245));
    face.setOutlineThickness(2.f);
    face.setOutlineColor(sf::Color(200, 200, 200));
    window.draw(face);

    // which cells of a 3x3 grid get a pip, for values 1 to 6
    static const int pips[7][9] = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 1, 0, 0, 0, 1},
        {1, 0, 1, 0, 0, 0, 1, 0, 1},
        {1, 0, 1, 0, 1, 0, 1, 0, 1},
        {1, 0, 1, 1, 0, 1, 1, 0, 1},
    };

    float pipRadius = size / 11.f;
    float cell = size / 4.f;
    for (int i = 0; i < 9; i++) {
        if (!pips[value][i]) {
            continue;
        }
        int row = i / 3;
        int col = i % 3;
        sf::CircleShape pip(pipRadius);
        pip.setOrigin(pipRadius, pipRadius);
        pip.setPosition(x + cell * (col + 1), y + cell * (row + 1));
        pip.setFillColor(sf::Color(30, 30, 30));
        window.draw(pip);
    }
}

// one dashboard card for a player in the side panel
void drawPlayerCard(sf::RenderWindow& window, const sf::Font& font, const Player& player,
                    Property* boardHead, sf::Color color, const string& label,
                    float y, bool isActive) {
    const float height = 125.f;
    sf::Color outline = isActive ? GOLD : sf::Color(80, 80, 85);
    drawRect(window, PANEL_X, y, PANEL_W, height, sf::Color(45, 45, 48), outline, 2.f);

    drawToken(window, font, PANEL_X + 26, y + 26, color, label);
    drawText(window, font, player.name, 22, PANEL_X + 50, y + 11, isActive ? GOLD : TEXT_MAIN, true);
    if (isActive) {
        drawText(window, font, "UP NEXT", 13, PANEL_X + PANEL_W - 72, y + 17, GOLD, true);
    }

    sf::Color moneyColor = player.money >= 0 ? sf::Color(110, 220, 120) : sf::Color(235, 90, 90);
    drawText(window, font, "Balance: $" + to_string(player.money), 19, PANEL_X + 16, y + 46, moneyColor);
    drawText(window, font, "On: " + player.currentPosition->name, 15, PANEL_X + 16, y + 76, TEXT_MAIN);
    drawText(window, font, "Properties owned: " + to_string(countOwned(boardHead, player.name)),
             14, PANEL_X + 16, y + 100, TEXT_DIM);
}

int main() {
    srand(time(0));
    Property* boardHead = nullptr;
    createBoard(boardHead);
    Player player1 = {"Player 1", 1500, boardHead};
    Player player2 = {"Player 2", 1500, boardHead};

    const int MAX_TURNS = 10;
    GameState state;

    // walk the circular list once so each property knows its spot on the board
    vector<Property*> boardTiles;
    Property* walker = boardHead;
    do {
        boardTiles.push_back(walker);
        walker = walker->next;
    } while (walker != boardHead);

    // 1. Create the Window
    sf::RenderWindow window(sf::VideoMode(WINDOW_W, WINDOW_H), "SDSU Monopoly - CS 210",
                            sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60);

    // 2. Load a Font (Required to draw text in SFML)
    // look for a local arial.ttf first, then fall back to the Windows system font
    sf::Font font;
    bool hasLocalFont = ifstream("arial.ttf").good();
    if (!(hasLocalFont && font.loadFromFile("arial.ttf")) && !font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
        cerr << "Could not load a font. Put arial.ttf next to the executable." << endl;
        return -1;
    }

    int currentTurn = 1;
    bool isPlayer1sTurn = true;

    // 3. The GUI Event Loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            // Close the window if the user clicks the 'X'
            if (event.type == sf::Event::Closed)
                window.close();

            // Trigger a turn when the Spacebar is pressed
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
                if (currentTurn <= MAX_TURNS) {
                    if (isPlayer1sTurn) {
                        takeTurn(player1, player2, state);
                    } else {
                        takeTurn(player2, player1, state);
                        currentTurn++; // Only advance the turn counter after both players go
                    }
                    isPlayer1sTurn = !isPlayer1sTurn;
                }
            }
        }

        bool gameOver = currentTurn > MAX_TURNS;

        // 4. Draw the Screen
        window.clear(BG); // Dark grey background

        // ---- The board ----
        const float boardCenterX = BOARD_X + TILE * 2.5f;
        drawRect(window, BOARD_X + TILE, BOARD_Y + TILE, TILE * 3, TILE * 3, CENTER_BG);

        for (int i = 0; i < (int)boardTiles.size(); i++) {
            drawTile(window, font, boardTiles[i], i, player1, boardTiles[i] == state.lastLanded);
        }

        // tokens sit side by side so both are visible when sharing a tile
        sf::Vector2f p1Tile = tilePosition(indexOf(boardTiles, player1.currentPosition));
        sf::Vector2f p2Tile = tilePosition(indexOf(boardTiles, player2.currentPosition));
        drawToken(window, font, p1Tile.x + TILE / 2.f - 18, p1Tile.y + 100, P1_COLOR, "1");
        drawToken(window, font, p2Tile.x + TILE / 2.f + 18, p2Tile.y + 100, P2_COLOR, "2");

        // ---- Center of the board: title, dice, prompt ----
        drawCentered(window, font, "SDSU", 60, boardCenterX, BOARD_Y + TILE + 18, SDSU_RED, true);
        drawCentered(window, font, "MONOPOLY", 36, boardCenterX, BOARD_Y + TILE + 92, TEXT_MAIN, true);
        drawCentered(window, font, "Campus Edition  -  CS 210", 15, boardCenterX, BOARD_Y + TILE + 140, TEXT_DIM);

        const float dieSize = 72.f;
        const float diceY = BOARD_Y + TILE + 180;
        drawDie(window, boardCenterX - dieSize - 10, diceY, dieSize, state.die1);
        drawDie(window, boardCenterX + 10, diceY, dieSize, state.die2);
        if (state.die1 > 0) {
            drawCentered(window, font, "Rolled " + to_string(state.die1 + state.die2), 18,
                         boardCenterX, diceY + dieSize + 10, TEXT_MAIN);
        }

        if (gameOver) {
            string result;
            if (player1.money == player2.money) {
                result = "It's a tie at $" + to_string(player1.money);
            } else {
                const Player& winner = player1.money > player2.money ? player1 : player2;
                result = "Winner: " + winner.name + " ($" + to_string(winner.money) + ")";
            }
            drawCentered(window, font, "Game Over!", 28, boardCenterX, BOARD_Y + TILE * 4 - 72, GOLD, true);
            drawCentered(window, font, result, 20, boardCenterX, BOARD_Y + TILE * 4 - 36, GOLD, true);
        } else {
            drawCentered(window, font, "Press [SPACEBAR] to Take Next Turn", 18,
                         boardCenterX, BOARD_Y + TILE * 4 - 40, TEXT_DIM);
        }

        // ---- Side panel: header ----
        int shownTurn = gameOver ? MAX_TURNS : currentTurn;
        drawText(window, font, "Turn: " + to_string(shownTurn) + " / " + to_string(MAX_TURNS),
                 26, PANEL_X, BOARD_Y, TEXT_MAIN, true);
        if (gameOver) {
            drawText(window, font, "Game finished", 18, PANEL_X, BOARD_Y + 38, GOLD);
        } else {
            const Player& up = isPlayer1sTurn ? player1 : player2;
            drawText(window, font, up.name + "'s turn", 18, PANEL_X, BOARD_Y + 38,
                     isPlayer1sTurn ? P1_COLOR : P2_COLOR, true);
        }

        // ---- Side panel: player cards ----
        drawPlayerCard(window, font, player1, boardHead, P1_COLOR, "1", 90, !gameOver && isPlayer1sTurn);
        drawPlayerCard(window, font, player2, boardHead, P2_COLOR, "2", 225, !gameOver && !isPlayer1sTurn);

        // ---- Side panel: activity log, newest first ----
        const float logY = 362, logH = BOARD_Y + TILE * 5 - logY;
        drawRect(window, PANEL_X, logY, PANEL_W, logH, sf::Color(45, 45, 48), sf::Color(80, 80, 85), 2.f);
        drawText(window, font, "Activity Log", 18, PANEL_X + 16, logY + 10, GOLD, true);

        float lineY = logY + 42;
        const float lineHeight = 20;
        if (state.log.empty()) {
            drawText(window, font, "No moves yet.", 15, PANEL_X + 16, lineY, TEXT_DIM);
        }
        for (int i = (int)state.log.size() - 1; i >= 0; i--) {
            bool newest = (i == (int)state.log.size() - 1);
            vector<string> lines = wrapText(state.log[i], font, 15, PANEL_W - 32);
            // stop before running past the bottom of the panel
            if (lineY + lines.size() * lineHeight > logY + logH - 8) {
                break;
            }
            for (const string& line : lines) {
                drawText(window, font, line, 15, PANEL_X + 16, lineY, newest ? TEXT_MAIN : TEXT_DIM);
                lineY += lineHeight;
            }
            lineY += 8;
        }

        window.display();
    }

    // free the circular board
    Property* current = boardHead->next;
    while (current != boardHead) {
        Property* nextProperty = current->next;
        delete current;
        current = nextProperty;
    }
    delete boardHead;

    return 0;
}
