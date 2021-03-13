#include "Player.h"
#include "Game.h"
#include "Timer.h"
#include "Board.h"
#include "FruitManager.h"
#include "Utils.h"

namespace Snake {

    Player::Player(string imgFile, int id, bool human) : GameObject(0,0,false,false) {
        infoPanel = nullptr;
        this->id = id;
        this->human = human;
        texture = Snake::LoadTexture(imgFile);
        if(!human)
            ai = make_unique<AI>();
        Init(InitLevel::NewPlayer);
    }

    Player::~Player() {
        SDL_DestroyTexture(texture);
    }

    bool Player::operator==(const Player& other) {
        return this->id == other.id;
    }

    void Player::Init(InitLevel initLevel) {
        // mind the missing breaks in the below switch
        switch (initLevel) {
            case InitLevel::NewPlayer:
                lives = INITIAL_LIVES;
                score = 0;
                wins = 0;
            case InitLevel::NewLevel:
                levelScore = 0;
            case InitLevel::Respawn:
                growBy = 0;
                bodySize = 0;
                delay = 0;
                speed = INITIAL_SPEED;
                turning = false;
        }
        status = PlayerStatus::Idle;
    }

    void Player::EraseFromBoard() {
        for (size_t i = 0; i < sBody.size(); i++) { // clear the board from player's positition
            Game::Gameboard->SetCell(sBody[i].boardCellX, sBody[i].boardCellY, CHAR_EMPTY); // (char)Board::CellKind::Empty);
        }
        sBody.clear();
    }

    void Player::PutOnBoard(int x, int y) {
        sBody.clear();
        SnakePart p;
        // Head
        p.boardCellX = x;
        p.boardCellY = y;
        p.rect = Game::Gameboard->GetCellRect(p.boardCellX, p.boardCellY);
        p.kind = CHAR_HEAD;
        p.direction = Direction::Right;
        p.clockwise = true;
        sBody.push_back(p);
        Game::Gameboard->SetCell(p.boardCellX, p.boardCellY, p.kind);
        // Body
        p.boardCellX = sBody[0].boardCellX - 1;
        p.boardCellY = sBody[0].boardCellY;
        p.rect = Game::Gameboard->GetCellRect(p.boardCellX, p.boardCellY);
        p.kind = CHAR_BODY;
        p.direction = Direction::Right;
        p.clockwise = true;
        sBody.push_back(p);
        Game::Gameboard->SetCell(p.boardCellX, p.boardCellY, p.kind);
        // Tail
        p.boardCellX = sBody[0].boardCellX - 2;
        p.boardCellY = sBody[0].boardCellY;
        p.rect = Game::Gameboard->GetCellRect(p.boardCellX, p.boardCellY);
        p.kind = CHAR_TAIL;
        p.direction = Direction::Right;
        p.clockwise = true;
        sBody.push_back(p);
        Game::Gameboard->SetCell(p.boardCellX, p.boardCellY, p.kind);
    }

    void Player::Update() {
        updateInfoPanel();
        if(!active)
            return;
        delay += speed;
        if(delay >= 100 || turning) {
            delay = 0;
            if(!human) {
                Direction newDir;
                AI::Node* goal = ai->GetGoal();
                if(!ai->HasPath() || Game::Gameboard->GetCell(goal->x, goal->y) != CHAR_FRUIT)
                    ai->FindShortestPath({ sBody[0].boardCellX, sBody[0].boardCellY });
                newDir = static_cast<Direction>(ai->GetNextMove());
                if(getNextCellForHead(newDir) != CHAR_EMPTY
                        && getNextCellForHead(newDir) != CHAR_FRUIT) {
                    ai->FindShortestPath({ sBody[0].boardCellX, sBody[0].boardCellY });
                    newDir = static_cast<Direction>(ai->GetNextMove());
                }
                Turn(newDir);
            }
            if(growBy > 0) { // if we must grow
                SnakePart part;
                int tailPos = sBody.size()-1; // tail Position
                part = sBody[tailPos];
                part.kind = CHAR_BODY;
                sBody.insert(sBody.begin() + tailPos, part); // add on tail
            }
            for(int i = sBody.size() - 1; i > 0; i--) { // proceed the body towards head
                if(sBody[i].kind==CHAR_TAIL && growBy > 0) // if we are growing we don't move the tail
                    continue;
                if(sBody[i].kind==CHAR_TAIL)
                    Game::Gameboard->SetCell(sBody[i].boardCellX, sBody[i].boardCellY, CHAR_EMPTY); // erase after tail
                sBody[i].rect = sBody[i - 1].rect;
                sBody[i].boardCellX = sBody[i - 1].boardCellX;
                sBody[i].boardCellY = sBody[i - 1].boardCellY;
                sBody[i].direction = sBody[i - 1].direction;
                sBody[i].clockwise = sBody[i - 1].clockwise;
                if(sBody[i].kind == CHAR_CURVE) // if the part that is moving is a curve
                    sBody[i].kind = CHAR_BODY; // transform it to body
                if(sBody[i - 1].kind == CHAR_CURVE && sBody[i].kind != CHAR_TAIL) // if the pos we are moving to belongs to a curve
                    sBody[i].kind = sBody[i - 1].kind; // transform moving part to curve
                if(turning && i == 1) { // last part moved that must be transformed to curve
                    turning = false;
                    sBody[i].clockwise = clockwise;
                    if(sBody[i].kind != CHAR_TAIL) // if it is the tail we do not transform it
                        sBody[i].kind = CHAR_CURVE;
                }
            }
            if(growBy > 0) {
                growBy--;
            }
            int offsetX = 0, offsetY = 0;
            switch(sBody[0].direction) {
                case Direction::Right:
                    offsetX = 1;
                    break;
                case Direction::Left:
                    offsetX = -1;
                    break;
                case Direction::Down:
                    offsetY = 1;
                    break;
                case Direction::Up:
                    offsetY = -1;
                    break;                    
                default:
                    break;
            }
            int newX = sBody[0].boardCellX + offsetX;
            int newY = sBody[0].boardCellY + offsetY;
            if(!Game::Gameboard->CheckLimits(newX, newY)) { // if new position is out of border limits
                newX = sBody[0].boardCellX;
                newY = sBody[0].boardCellY;
            }
            if(Game::Gameboard->GetCell(newX, newY) == CHAR_FRUIT) { // if we ate a fruit
                Fruit& fruitEaten = FruitManager::GetInstance().GetFruitAt(newX, newY);
                FruitEatenEvent(fruitEaten);
            }
            sBody[0].rect = Game::Gameboard->GetCellRect(newX, newY); // draw snake head at new pos
            if(!checkCollisions(newX, newY)) { // if we didn't hit anything
                sBody[0].boardCellX = newX; // update snake head
                sBody[0].boardCellY = newY; // with new pos
                for(auto& c : sBody) { // update board with snake parts
                    Game::Gameboard->SetCell(c.boardCellX, c.boardCellY, c.kind);
                }
            } else {
                CrashedEvent();
            }
        }
    }



    void Player::Render() {
        if(!visible)
            return;
        SDL_Rect src = {0, 0, 64, 64};
        for(size_t i=0; i<sBody.size(); i++) {
            if(sBody[i].kind==CHAR_HEAD) { // Head
                switch(sBody[i].direction) {
                    case Direction::Right:
                        src.x = 256;
                        src.y = 0;
                        break;
                    case Direction::Left:
                        src.x = 192;
                        src.y = 64;
                        break;
                    case Direction::Down:
                        src.x = 256;
                        src.y = 64;            
                        break;
                    case Direction::Up:
                        src.x = 192;
                        src.y = 0;                    
                        break;
                    default:
                        break;
                }
            }
            if(sBody[i].kind==CHAR_BODY) { // Body
                switch(sBody[i].direction) {
                    case Direction::Right:
                    case Direction::Left:
                        src.x = 64;
                        src.y = 0;
                        break;
                    case Direction::Down:
                    case Direction::Up:
                        src.x = 128;
                        src.y = 64;                    
                        break;
                    default:
                        break;
                }
            }
            if(sBody[i].kind==CHAR_CURVE) { // Curve
                if(sBody[i].clockwise) {
                    switch(sBody[i].direction) {
                        case Direction::Right:
                            src.x = 0;
                            src.y = 0;
                            break;
                        case Direction::Left:
                            src.x = 128;
                            src.y = 128;
                            break;
                        case Direction::Down:
                            src.x = 128;
                            src.y = 0;            
                            break;
                        case Direction::Up:
                            src.x = 0;
                            src.y = 64;                    
                            break;
                        default:
                            break;
                    }
                } else {
                    switch(sBody[i].direction) {
                        case Direction::Down:
                            src.x = 0;
                            src.y = 0;
                            break;
                        case Direction::Up:
                            src.x = 128;
                            src.y = 128;
                            break;
                        case Direction::Left:
                            src.x = 128;
                            src.y = 0;            
                            break;
                        case Direction::Right:
                            src.x = 0;
                            src.y = 64;                    
                            break;
                        default:
                            break;
                    }
                }
                
            }
            if(sBody[i].kind==CHAR_TAIL) { // Tail
                switch(sBody[i].direction) {
                    case Direction::Right:
                        src.x = 256;
                        src.y = 128;
                        break;
                    case Direction::Left:
                        src.x = 192;
                        src.y = 192;
                        break;
                    case Direction::Down:
                        src.x = 256;
                        src.y = 192;                    
                        break;
                    case Direction::Up:
                        src.x = 192;
                        src.y = 128;
                        break;
                    default:
                        break;
                }
            }
            SDL_RenderCopy(Game::Renderer, texture, &src, &sBody[i].rect);
            if(infoPanel && infoPanel->IsVisible())
                infoPanel->Render();
#ifdef DEBUG
            if(!human)
            {
                SDL_Rect rect;
                for(auto p : ai->nodesToShow)
                {
                    rect = Game::Gameboard->GetCellRect(p->x, p->y);
                    SDL_SetRenderDrawColor(Game::Renderer, 255, 0, 0, 255);
                    SDL_RenderDrawRect(Game::Renderer, &rect);
                }
            }
#endif
        }
    }

    void Player::PauseStartTimer() {
        if(startTimer != nullptr && startTimer->IsCounting()) {
            status = PlayerStatus::Idle;
            startTimer->Stop();
        }
    }

    void Player::CreateInfoPanel(int x, int y, size_t w, size_t h, SDL_Color bgColor, SDL_Color fgColor, SDL_GUI::HorizontalAlign hAlign, SDL_GUI::VerticalAlign vAlign)
    {
        std::string fnt = "assets/";
        fnt.append(SDL_GUI::sFonts::TTF_TIMES);
        infoPanel = make_unique<UIPanel>(Game::Renderer, "plrINFO", x, y, w, h, false, true, 
                                         fnt, 24,
                                         hAlign, vAlign, vAlign, bgColor, fgColor);
        infoPanel->AddItem("plr_name", this->Who());
        infoPanel->AddItem("plr_speed", "Speed: 0");
        infoPanel->AddItem("plr_size", "Size: 0");
        infoPanel->AddItem("plr_lives", "Lives: 0");
        infoPanel->AddItem("plr_lvl_score", "Score: 0");
        infoPanel->AddItem("plr_wins", "Wins: 0");
        infoPanel->AddItem("plr_status", "Idle...");
        infoPanel->Show();
    }

	int Player::GetId() const {
		return id;
	}

	string Player::Who() const {
		return "Player " + to_string(id + 1);
	}

    void Player::Turn(Direction newDir) {
        if(!active)
            return;
        if(canChangeDirection(newDir)) {
            if(((int8_t)newDir - (int8_t)sBody[0].direction == 1) || 
                ((int8_t)newDir - (int8_t)sBody[0].direction == -3))
                    clockwise = true;
            else
                    clockwise = false;            
            sBody[0].direction = newDir;
            turning = true;
        }
    }

    void Player::Grow(int grow) {
        if(!active || bodySize >= MAX_BODY)
            return;
        growBy += min(MAX_BODY - bodySize, grow);
        bodySize += growBy;
    }

    void Player::SpeedUp(int speedUpBy) {
        if(!active)
            return;
        speed += speedUpBy;
        if(speed > MAX_SPEED)
            speed = MAX_SPEED;
    }

    int Player::GetSpeed() const {
        return speed;
    }

    int Player::GetScore() const {
        return score;
    }

    int Player::GetSize() const {
        return bodySize;
    }

    int Player::GetLevelScore() const {
        return levelScore;
    }

    int Player::GetWins() const {
        return wins;
    }

    void Player::MakeAWin() {
        wins++;
    }

    bool Player::IsCrashed() const {
        return status == PlayerStatus::Crashed;
    }

    bool Player::IsDead() const {
        return status == PlayerStatus::Dead;
    }

    bool Player::IsHuman() const {
        return human;
    }

    bool Player::IsStarting() const {
        return status == PlayerStatus::Starting;
    }

    bool Player::IsPlaying() const {
        return status == PlayerStatus::Playing;
    }

    uint8_t Player::GetLives() const {
        return lives;
    }

    void Player::CrashedEvent() {
        if(lives > 0) {
            lives--;
            status = PlayerStatus::Crashed;
        } else {
            status = PlayerStatus::Dead;
        }
        for(auto& p : sBody) {
            Game::Gameboard->SetCell(p.boardCellX, p.boardCellY, CHAR_EMPTY);
        }
        Deactivate();
        if(!human)
            ai->ClearPath();
        OnCrashedEvent(shared_from_this());
    }

    void Player::Activate() {
        if(!startTimer) {
            startTimer = make_unique<Timer>(3);
            startTimer->OnTimeOutEvent.AddListener(bind(&Player::ActivatedEvent, this));
        } else {
            startTimer->Reset(3);
        }
        startTimer->Start();
        status = PlayerStatus::Starting;
    }

    void Player::ActivatedEvent() {
        GameObject::Activate();
        status = PlayerStatus::Playing;
    }

    void Player::Deactivate() {
        if(startTimer != nullptr)
            startTimer->Stop();
        GameObject::Deactivate();
    }

    void Player::Pause() {
        prevStatus = status;
        wasActive = active;
        Deactivate();
        status = PlayerStatus::Paused;
    }

    void Player::UnPause() {
        active = wasActive;
        status = prevStatus;
        if(IsStarting())
            startTimer->Start();
    }

    int Player::TimeToStart() const {
        if(startTimer == nullptr)
            return 0;
        return startTimer->GetRemainingTime();
    }

    void Player::FruitEatenEvent(Fruit& fruit) {
        Grow(fruit.GrowBy());
        SpeedUp(2);
        score++;
        levelScore++;
        OnFruitEatenEvent(shared_from_this(), fruit);
    }

    void Player::updateInfoPanel()
    {
        if(!infoPanel || !infoPanel->IsVisible())
            return;
        (*infoPanel)["plr_speed"]->SetText("Speed : " + to_string(speed));
        (*infoPanel)["plr_size"]->SetText("Size : " + to_string(bodySize));
        (*infoPanel)["plr_lives"]->SetText("Lives : " + to_string(lives));
        if(lives <= 1)
            (*infoPanel)["plr_lives"]->SetColor(DARK_GREEN_COLOR, RED_COLOR);
        (*infoPanel)["plr_lvl_score"]->SetText("Score : " + to_string(score));
        (*infoPanel)["plr_wins"]->SetText("Wins : " + to_string(wins));
        switch(status) {
            case PlayerStatus::Starting:
                (*infoPanel)["plr_status"]->SetText("Starting in " + to_string(startTimer->GetRemainingTime()));
                break;
            case PlayerStatus::Dead:
                (*infoPanel)["plr_status"]->SetText("DEAD!!!");
                break;
            case PlayerStatus::Crashed:
                (*infoPanel)["plr_status"]->SetText("CRASHED!!!");
                break;
            case PlayerStatus::Paused:
                (*infoPanel)["plr_status"]->SetText("PAUSED!!!");
                break;
            default:
                (*infoPanel)["plr_status"]->SetText("");
                break;
        }
    }

    bool Player::checkCollisions(int x, int y) {
        return Game::Gameboard->GetCell(x, y) != CHAR_EMPTY;
    }

    bool Player::canChangeDirection(Direction newDir) {
        // opposite directions have a difference of 2
        return (abs((uint8_t)newDir - (uint8_t)sBody[0].direction) != 2 && 
                newDir != sBody[0].direction);
    }

    char Player::getNextCellForHead(Direction newDir) const
    {
        int offsetX = 0, offsetY = 0;
        switch(newDir)
        {
            case Direction::Right:
                offsetX = 1;
                break;
            case Direction::Left:
                offsetX = -1;
                break;
            case Direction::Down:
                offsetY = 1;
                break;
            case Direction::Up:
                offsetY = -1;
                break;
            default:
                break;
        }
        return Game::Gameboard->GetCell(sBody[0].boardCellX + offsetX, sBody[0].boardCellY + offsetY);
    }

}