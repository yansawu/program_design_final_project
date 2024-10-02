#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<time.h>

void creat_a_map(int **map,int *map_row,int *map_column);
void setup_village(int **map,int map_row,int map_column);
void setup_monster(int **map, int map_row, int map_column);
void setup_player(int **map, int map_row, int map_column, int *player_current_row, int *player_current_column);
void setup_mines(int **map, int map_row, int map_column);
void print_map(int **map, int map_row, int map_column, bool *mine_detector);
void print_state(int player_blood, int player_strength, int player_money);
void creat_a_whole_map(int **map, int *map_row, int *map_column, int *player_current_row, int *player_current_column, bool *gun_exist, bool *mine_detector);
void go_up(int **map, int map_row, int map_column, int *player_current_row, int *player_current_column);
void go_down(int **map, int map_row, int map_column, int *player_current_row, int *player_current_column);
void go_right(int **map, int map_row, int map_column, int *player_current_row, int *player_current_column);
void go_left(int **map, int map_row, int map_column, int *player_current_row, int *player_current_column);
void move(int **map, int map_row, int map_column, int *player_current_row, int *player_current_column, int *player_strength, int *player_blood, int *player_money, bool *gun_exist, bool *can_go, int *total_monster, bool *play, bool *totem_of_undying, bool *mine_detector,bool *knife, bool *sword, bool *Excalibar);
void village_npc(int **map,int map_row,int map_column,int *player_blood, int *player_strengh, int *money, int total_monster, bool *play, bool *knife, bool *sword, bool *Excalibar, bool *totem_of_undying, bool *mine_detector);
void weapon_shop(int *player_strengh, int *money, bool *knife, bool *sword, bool *Excalibar);
void special_item_shop(int *money, bool *totem_of_undying, bool *mine_detector);
void lotion_shop(int *player_strengh, int *player_blood, int *money);
void setup_gun(int **map, int map_row, int map_column, bool *gun_exist);
void objects_interaction(int **map, int map_row, int map_column, int player_current_row, int player_current_column, int *player_strength, int *player_blood, int *player_money, bool *gun_exist, bool *can_go, int *total_monster, int *player_strengh, bool *play, bool *knife, bool *sword, bool *Excalibar, bool *totem_of_undying,bool *mine_detector);
void check_die_or_win(int **map,int map_row,int map_column,int *player_blood, int total_monster, bool *play, bool *totem_of_undying);
bool check_boundary(int input_row, int input_column,int map_row,int map_column);
bool check_occupied(int **map,int input_row, int input_column);
bool dodge_probability(int **map,int monster_row,int monster_column);
int randon_number(int initial, int end);

int main(){
    srand(time(NULL));
    int **map = (int **) malloc(1000 *sizeof(int));
    for (int i = 0; i < 1000;i++){
        map[i] = (int *)malloc(1000 * sizeof(int));
    } //declare a 2-dimension array
    int map_row, map_column;  //the size of map
    int player_blood = 15, player_strengh = 5, player_money = 10, player_current_row = 0, player_current_column = 0;
    bool gun_exist = true, can_go = false, play = true;
    bool knife = false, sword = false, Excalibar = false;
    bool totem_of_undying = false, mine_detector = false;
    creat_a_whole_map(map, &map_row, &map_column, &player_current_row, &player_current_column, &gun_exist, &mine_detector);

    int total_monster = (map_row * map_column) / 4;

    while(play){
        move(map, map_row, map_column, &player_current_row, &player_current_column, &player_strengh, &player_blood, &player_money, &gun_exist, &can_go, &total_monster, &play, &totem_of_undying, &mine_detector, &knife, &sword, &Excalibar);
    }
    
    free(map);
    return 0;
}

void creat_a_map(int **map,int *map_row,int *map_column){
    printf("Input the size of map: (row column)\n");
    scanf("%d%d", map_row, map_column);
    for (int i = 0; i < *map_row;i++){
        for (int j = 0; j < *map_column;j++){
            map[i][j] = 0;  //empty place will displayed by 0
        }
    }
}

void print_map(int **map,int map_row,int map_column, bool *mine_detector){ 
    for (int i = 0; i < map_row;i++){
        for (int j = 0; j < map_column;j++){
            if(map[i][j]==0)
                printf(". ");
            if(map[i][j]==1)
                printf("\033[46;37mP\033[0m "); //blue base
            if(map[i][j]==2)
                printf("\033[45;37mV\033[0m "); //purple base
            if(map[i][j]==5)
                printf("\033[40;37mm\033[0m "); //white
            if(map[i][j]==6)
                printf("\033[40;34mm\033[0m "); //blue
            if(map[i][j]==7)
                printf("\033[40;33mm\033[0m "); //yellow
            if(map[i][j]==8)
                printf("\033[40;31mm\033[0m "); //red
            if(map[i][j]==11)
                printf("G ");
            if(map[i][j]==12){
                if(*mine_detector){
                    printf("- ");
                }
                else
                    printf(". ");
            }
        }
        printf("\n");
    }
}

void setup_village(int **map,int map_row,int map_column){
    int village_row, village_column;
    printf("Input the location for the villige:(row column)\n");
    do{
    scanf("%d%d", &village_row, &village_column);
    } while (check_boundary(village_row, village_column, map_row, map_column) || check_occupied(map, village_row, village_column));
    map[village_row][village_column] = 2;  //villige location will displayed by 2 
}

void setup_player(int **map,int map_row,int map_column,int *player_current_row, int *player_current_column){
    do{
    printf("Adventure Time! Choose a place to start :(row column)\n");
    scanf("%d%d", player_current_row, player_current_column);
    } while (check_boundary(*player_current_row, *player_current_column, map_row, map_column) || check_occupied(map, *player_current_row, *player_current_column));
    map[*player_current_row][*player_current_column] = 1;
    printf("Player are ready to go!\n");
}

//auto set monsters in the map
void setup_monster(int **map,int map_row, int map_column){
    int total_monster = (map_row * map_column) / 4;
    printf("You have to defeat %d monsters in total\n", total_monster);
    while(total_monster!=0){
        int temprow = randon_number(0, map_row - 1), tempcolumn = randon_number(0, map_column - 1);
        if(map[temprow][tempcolumn]!=0){
            continue;
        }
        else{
            map[temprow][tempcolumn] = randon_number(5, 8); //level of the monsters
            total_monster--;
        }
    }
    printf("\033[40;33mMonsters are setuped already!\033[0m\n");
}

void creat_a_whole_map(int **map, int *map_row, int *map_column, int *player_current_row, int *player_current_column,bool *gun_exist, bool *mine_detector){
    bool flag1 = true, flag2;
    int action;
    while(flag1){
        flag2 = true;
        creat_a_map(map, map_row, map_column);
        setup_village(map, *map_row, *map_column);
        setup_player(map, *map_row, *map_column, player_current_row, player_current_column);
        setup_monster(map, *map_row, *map_column);
        setup_gun(map, *map_row, *map_column, gun_exist);
        setup_mines(map, *map_row, *map_column);
        print_map(map, *map_row, *map_column, mine_detector);
        do{
            printf("[1]:reset the map [2]:start the journy ==> ");
            scanf("%d", &action);
            switch(action){
                case 1:
                    flag2 = false;
                    break;
                case 2:
                    flag1 = false;
                    flag2 = false;
                    break;
                default: //if input another number
                    printf("\033[40;31mInput again.\033[0m\n ");
                    break;
            }
        } while (flag2);
    }
}

bool check_boundary(int input_row,int input_column,int map_row,int map_column){     //check weather the input out of the map
    if(input_row<0||input_row>map_row-1||input_column<0||input_column>map_column-1){
        printf("\033[40;31mOh no! The place is not exist!\nInput again!\n\033[0m ");
        return 1;
    }
    else
        return 0;
}

bool check_occupied(int **map,int input_row, int input_column){         //check weather the place occupied
    if(map[input_row][input_column]!=0){
        printf("\033[40;31mOh no! The place is occupied!\nInput again!\n\033[0m ");
        return true;
    }
    else 
        return false;
}

int randon_number(int initial, int end){        //randonly select a number in range (initial)~(end) 
    int randon = rand() % (end - initial + 1) + initial;
    return randon;
}

void go_up(int **map, int map_row, int map_column,int *player_current_row, int *player_current_column){
    if(check_boundary(*player_current_row-1, *player_current_column,map_row, map_column)){
        
    }
    
    else{
        map[*player_current_row][*player_current_column] = 0;
        map[*player_current_row-1][*player_current_column] = 1;
        *player_current_row = *player_current_row - 1;
    }
}

void go_down(int **map, int map_row, int map_column,int *player_current_row, int *player_current_column){
    if(check_boundary(*player_current_row+1, *player_current_column, map_row, map_column)){
    }
    else{
        map[*player_current_row][*player_current_column] = 0;
        map[*player_current_row+1][*player_current_column] = 1;
        *player_current_row = *player_current_row + 1;
    }
}

void go_right(int **map, int map_row, int map_column,int *player_current_row, int *player_current_column){
    if(check_boundary(*player_current_row, *player_current_column+1 ,map_row, map_column)){
    }
    
    
    else{
        map[*player_current_row][*player_current_column] = 0;
        map[*player_current_row][*player_current_column+1] = 1;
        *player_current_column = *player_current_column + 1;
    }
}

void go_left(int **map, int map_row, int map_column,int *player_current_row, int *player_current_column){
    if(check_boundary(*player_current_row, *player_current_column-1, map_row, map_column)){
    }
    
    else{
        map[*player_current_row][*player_current_column] = 0;
        map[*player_current_row][*player_current_column-1] = 1;
        *player_current_column = *player_current_column - 1;
    }
}

void village_npc(int **map,int map_row, int map_column, int *player_blood, int *player_strengh, int *money, int total_monster, bool *play, bool *knife, bool *sword, bool *Excalibar, bool *totem_of_undying, bool *mine_detector){
    int village_choice;
    bool flag = true;
    printf("\033[40;32mWelcome to our villige!\033[0m\n");
    while(flag){
        printf("\033[40;32mDo you need any help?\n\033[0m\n");
        printf("[1]:weapons shop [2]:special item shop [3]:lotion shop [4]:back to journy\n==>");
        scanf("%d", &village_choice);
        switch(village_choice){
            case 1:
                weapon_shop(player_strengh, money, knife, sword, Excalibar);
                break;
            case 2:
                special_item_shop(money, totem_of_undying, mine_detector);
                break;
            case 3:
                lotion_shop(player_strengh, player_blood, money);
                check_die_or_win(map, map_row, map_column, player_blood, total_monster, play, totem_of_undying);
                break;
            case 4:
                flag = false;
                printf("\033[40;32mSee you, hero!\033[0m\n");
                break;
            default:
                printf("\033[40;31mInvaliable choice! Please input again!!\033[0m\n");
                break;
        }
    }
}

void weapon_shop(int *player_strengh, int *money, bool *knife, bool *sword, bool *Excalibar){
    bool exit = true;
    int number;
    printf("\033[40;32mHey guy! I think you need some weapon.\033[0m\n");
    while(exit){
        printf("[1]:knife (+1 ATK)        $5\n[2]:sword (+3 ATK)        $18\n[3]:Excalibar (+10 ATK)   $50\n[4]:Oh no I should save some money QQ\n");
        printf("\033[40;32mCoin: $%d\033[0m\n", *money);
        printf("==>");
        scanf("%d", &number);
        switch(number){
            case 1:
                if(*knife){
                    printf("\033[40;33mYou have already buy this item.\033[0m\n");
                    break;
                }
                else if(*money<5){  //money is not enough
                    printf("\033[40;33mThis is not for free.\033[0m\n"); 
                    break;
                }
                else{
                    printf("\033[40;32mYou look more powerful.\033[0m\n");
                    *player_strengh = *player_strengh + 1;
                    *money = *money - 5;
                    *knife = true;
                    break;
                }
            case 2:
                if(*sword){
                    printf("\033[40;33mYou have already buy this item.\033[0m\n"); 
                    break;
                }
                else if(*money<18){   //money is not enough
                    printf("\033[40;33mThere is not charity!\033[0m\n"); 
                    break;
                }
                else{
                    printf("\033[40;32mYou look more powerful.\033[0m\n");  
                    *player_strengh = *player_strengh + 3;
                    *money = *money - 18;
                    *sword = true;
                    break;
                }
            case 3:
                if(*Excalibar){
                    printf("\033[40;33mYou have already buy this item.\033[0m\n"); 
                    break;
                }
                else if(*money<50){    //money is not enough
                    printf("\033[40;33mGet your dirty hands away!\033[0m\n"); 
                    break;
                }
                else{
                    printf("\033[40;32mYou are invicible now(?\033[0m\n"); 
                    *player_strengh = *player_strengh + 10;
                    *money = *money - 50;
                    *Excalibar = true;
                    break;
                }
            case 4:
                printf("\033[40;32mMay the Force be with you.\033[0m\n"); 
                exit = false;
                break;
            default:
                printf("\033[40;31mAre you an illiteracy?\033[0m\n"); 
                break;
        }       
    }
}

void special_item_shop(int *money, bool *totem_of_undying , bool *mine_detector ){
    printf("\033[40;32mMaybe these things can help you.\033[0m\n");
    int number;
    bool exit = true;
    while(exit){
        printf("[1]:Totem of Undying   $40\n[2]:mine_detector   $50\n[3]:exit\n==>");
        printf("\033[40;32mCoin: $%d\033[0m\n", *money);
        scanf("%d", &number);
        switch(number){
            case 1:
                if(*totem_of_undying){
                    printf("\033[40;33mYou have already buy this item.\033[0m\n"); 
                    break;
                }
                else if(*money<40){  //money is not enough
                    printf("\033[40;33mI'm watching.\033[0m\n");
                    break;
                }
                else{
                    printf("\033[40;32mNow you are undying!\033[0m\n");
                    *totem_of_undying = true;
                    *money = *money - 40;
                    break;
                }
            case 2:
                if(*mine_detector){
                    printf("\033[40;33mYou have already buy this item.\033[0m\n");
                    break;
                }
                else if(*money<50){   //money is not enough
                    printf("\033[40;33mGet your dirty hands away!\033[0m\n"); 
                    break;
                }
                else{
                    printf("\033[40;32mYou get the mine detector!!\033[0m\n");
                    *mine_detector = true;
                    *money = *money - 50;
                    break;
                }
            case 3:
                printf("\033[40;32mGood luck\033[0m\n"); 
                exit = false;
                break;
            default:
                printf("\033[40;31m: ......\033[0m\n");
                break;
        }
    }
}

void lotion_shop(int *player_strengh, int *player_blood, int *money){
    printf("\033[40;32mMagic is amazing!\033[0m\n"); 
    int number;
    bool exit = true;
    while(exit){
        printf("[1]:red lotion (HP+5)  $10\n[2]:big red lotion (HP Full)  $30\n[3]:colorful lotion   $15\n[4]:exit\n==>");
        printf("\033[40;32mCoin: $%d\033[0m\n", *money);
        scanf("%d", &number);
        switch(number){
            case 1:
                if(*money<10){
                    printf("\033[40;33mThis is not for free.\033[0m\n"); 
                    break;
                }
                else if(*player_blood==20){
                    printf("\033[40;33mPut it down! You don't need this.\033[0m\n"); 
                    break;
                }
                else{
                    *player_blood = *player_blood + 5;
                    if(*player_blood>20)
                        *player_blood = 20;
                    *money = *money - 10;
                    break;
                }
            case 2:
                if(*money<30){  //money is not enough
                    printf("\033[40;33mMagic is amazing, but money is more than that.\033[0m\n");
                    break;
                }
                else if(*player_blood==20){
                    printf("\033[40;33mDon't waste my lotion.\033[0m\n");
                    break;
                }
                else{
                    *money = *money - 40;
                    *player_blood = 20;
                    break;
                }
            case 3:
                if(*money<15){
                    printf("\033[40;33mGet your dirty hands away!\033[0m\n");
                    break;
                }
                else{
                    printf("\033[40;32mSomething is happen......\033[0m\n");
                    switch(randon_number(1,3)){
                        case 1:
                            printf("\033[40;34mI feel more powerful!\033[0m\n");
                            *player_strengh = *player_strengh + 2;
                            break;
                        case 2:
                            printf("\033[40;34mMy wound is getting better!\033[0m\n"); 
                            *player_blood = *player_blood + 8;
                            if(*player_blood>20)
                                *player_blood = 20;
                            break;
                        case 3:
                            printf("\033[40;33mWTF is this (＊Л＊)\033[0m\n");
                            *player_blood = *player_blood - 3;
                            break;
                        case 4:
                            printf("\033[40;33mI'm lossing my force Σ（ﾟдﾟlll）\033[0m\n");
                            *player_strengh = *player_strengh - 1;
                            break;
                    }
                }
            case 4:
                printf("\033[40;32mHave a nice day!\033[0m\n");
                exit = false;
                break;
            default:
                printf("\033[40;32m: ......\033[0m\n");
                break;
        }
    }
}

bool dodge_probability(int **map,int monster_row,int monster_column){ 
    int level = map[monster_row][monster_column];
    int random;
    bool dodge;
    switch(level){
        case 5:  //(1/8)probability to dodge the attack
            random = randon_number(1, 8);
            if (random<8){
                dodge = false;
                break;
            }
            else{
                dodge = true;
                break;
            }
        case 6:  //(1/7)probability to dodge the attack
            random = randon_number(1, 7);
            if (random<7){
                dodge = false;
                break;
            }
            else{
                dodge = true;
                break;
            }
        case 7:  //(1/6)probability to dodge the attack
            random = randon_number(1, 6);
            if (random<6){
                dodge = false;
                break;
            }
            else{
                dodge = true;
                break;
            }

        case 8:  //(1/5)probability to dodge the attack
            random = randon_number(1, 5);
            if (random<5){
                dodge = false;
                break;
            }
            else{
                dodge = true;
                break;
            }
    }
    return dodge;
}

void setup_mines(int **map, int map_row, int map_column){
    int total_mines = (map_row * map_column) / 8;
    printf("There are  %d mines in total\n", total_mines);
    while(total_mines!=0){
        int temprow=randon_number(0,map_row-1), tempcolumn=randon_number(0,map_column-1); 
        if(map[temprow][tempcolumn]!=0){
            continue;
        }
        else{
            map[temprow][tempcolumn] = 12; //12 means minnes
            total_mines--;
        }
    }
    printf("\033[40;33mMines are setuped already! Be careful!\033[0m\n");
}

void setup_gun(int **map, int map_row, int map_column, bool *gun_exist){
    while(*gun_exist){
        int temprow=randon_number(0,map_row-1), tempcolumn=randon_number(0,map_column-1); 
        if(map[temprow][tempcolumn]!=0){
            continue;
        }
        else{
            map[temprow][tempcolumn] = 11; //11 means gun
            *gun_exist=false;
        }
    }
}

void objects_interaction(int **map, int map_row, int map_column, int player_current_row, int player_current_column, int *player_strength, int *player_blood, int *player_money, bool *gun_exist, bool *can_go, int *total_monster, int *player_strengh, bool *play, bool *knife, bool *sword, bool *Excalibar, bool *totem_of_undying,bool *mine_detector){
    int object = map[player_current_row][player_current_column];
    switch(object){
        case 0: //empty place
            *can_go = true;
            break;
        case 2: //villige
            village_npc(map, map_row, map_column, player_blood, player_strengh, player_money, *total_monster, play, knife, sword, Excalibar, totem_of_undying, mine_detector);
            break;
        case 5:  //monster 5==> str:4,atk:5
            if(4>=*player_strength){
                printf("\033[40;31mYou lose!\nYou get %d points hurt\033[0m\n", object);
                *player_blood = *player_blood - object;
                break;
            }
            else if(dodge_probability(map,player_current_row,player_current_column)){
                printf("\033[40;31mThe monster dodged your attack!\n You get %d points hurt\033[0m\n", object);
                *player_blood = *player_blood - object;
                break;
            }
            else{
                printf("\033[40;32mThe monster is died. Good job!\nYou get $5.\033[0m\n");
                *player_money = *player_money + 5;
                *can_go = true;
                *total_monster = *total_monster - 1;
                break;
            }
        case 6:  //monster 6==> str:8,atk:6
            if(8>=*player_strength){
                printf("\033[40;31mYou lose!\nYou get %d points hurt\033[0m\n", object);
                *player_blood = *player_blood - object;
                break;
            }
            else if(dodge_probability(map,player_current_row,player_current_column)){
                printf("\033[40;31mThe monster dodged your attack!\n You get %d points hurt\033[0m\n", object);
                *player_blood = *player_blood - object;
                break;
            }
            else{
                printf("\033[40;32mThe monster is died. Good job!\nYou get $5.\033[0m\n");
                *player_money = *player_money + 8;
                *can_go = true;
                *total_monster = *total_monster - 1;
                break;
            }
        case 7:  //monster 7==> str:10,atk:7
            if(10>=*player_strength){
                printf("\033[40;31mYou lose!\nYou get %d points hurt\033[0m\n", object);
                *player_blood = *player_blood - object;
                break;
            }
            else if(dodge_probability(map,player_current_row,player_current_column)){
                printf("\033[40;31mThe monster dodged your attack!\n You get %d points hurt\033[0m\n", object);
                *player_blood = *player_blood - object;
                break;
            }
            else{
                printf("\033[40;32mThe monster is died. Good job!\nYou get $5.\033[0m\n");
                *player_money = *player_money + 12;
                *can_go = true;
                *total_monster = *total_monster - 1;
                break;
            }
        case 8:  //monster 8==> str:15,atk:8
            if(15>=*player_strength){
                printf("\033[40;31mYou lose!\nYou get %d points hurt\033[0m\n", object);
                *player_blood = *player_blood - object;
                break;
            }
            else if(dodge_probability(map,player_current_row,player_current_column)){
                printf("\033[40;31mThe monster dodged your attack!\n You get %d points hurt\033[0m\n", object);
                *player_blood = *player_blood - object;
                break;
            }
            else{
                printf("\033[40;32mThe monster is died. Good job!\nYou get $5.\033[0m\n");
                *player_money = *player_money + 15;
                *can_go = true;
                *total_monster = *total_monster - 1;
                break;
            }
        case 11:  //get gun ==> ATK+1
            printf("\033[40;35mCongratulation! You get a gun.\nATK +1\033[0m\n");
            *player_strength = *player_strength + 1;
            *gun_exist = true;
            setup_gun(map, map_row, map_column, gun_exist);
            *can_go = true;
            break;
        case 12:
            printf("\033[40;31mBoom!!!!!!!!!!!!\nHP - 1\033[0m\n");
            *player_blood = *player_blood - 1;
            *can_go = true;
            break;
        default:
            printf("\033[40;31mOh no! The place is not exist!\nInput again!\033[0m\n");
            break;
    }
}

void move(int **map, int map_row, int map_column, int *player_current_row, int *player_current_column, int *player_strength, int *player_blood, int *player_money, bool *gun_exist, bool *can_go, int *total_monster, bool *play, bool *totem_of_undying, bool *mine_detector,bool *knife, bool *sword, bool *Excalibar){
    int action;
    bool flag = true;
    printf("[5]:go up [2]:go down [3]:go right [1]:go left [9]:give up\n==> ");
    while(flag){
        scanf("%d", &action);
        switch(action){
            case 5:
                if(*player_current_row>=1)
                    objects_interaction(map, map_row, map_column, *player_current_row-1, *player_current_column, player_strength, player_blood, player_money, gun_exist, can_go, total_monster,player_strength,play,knife,sword,Excalibar,totem_of_undying,mine_detector);

                if(can_go){
                    go_up(map, map_row, map_column, player_current_row, player_current_column);
                }
                print_state(*player_blood, *player_strength, *player_money);
                print_map(map, map_row, map_column, mine_detector);
                *can_go = false;
                flag = false;
                check_die_or_win(map, map_row, map_column, player_blood, *total_monster, play, totem_of_undying);
                break;
            case 2: 
                objects_interaction(map, map_row, map_column, *player_current_row+1, *player_current_column, player_strength, player_blood, player_money, gun_exist, can_go, total_monster,player_strength,play,knife,sword,Excalibar,totem_of_undying,mine_detector);
                if(*can_go){
                    go_down(map, map_row, map_column, player_current_row, player_current_column);
                }
                print_state(*player_blood, *player_strength, *player_money);
                print_map(map, map_row, map_column, mine_detector);
                *can_go = false;
                flag = false;
                check_die_or_win(map, map_row, map_column, player_blood, *total_monster, play, totem_of_undying);
                break;
            case 3:
                objects_interaction(map, map_row, map_column, *player_current_row, *player_current_column+1, player_strength, player_blood, player_money, gun_exist, can_go, total_monster,player_strength,play,knife,sword,Excalibar,totem_of_undying,mine_detector);
                if(*can_go){
                    go_right(map, map_row, map_column, player_current_row, player_current_column);
                }
                print_state(*player_blood, *player_strength, *player_money);
                print_map(map, map_row, map_column, mine_detector);
                *can_go = false;
                flag = false;
                check_die_or_win(map, map_row, map_column, player_blood, *total_monster, play, totem_of_undying);
                break;
            case 1:
                if(*player_current_column>=1)
                    objects_interaction(map, map_row, map_column, *player_current_row, *player_current_column-1, player_strength, player_blood, player_money, gun_exist, can_go, total_monster,player_strength,play,knife,sword,Excalibar,totem_of_undying,mine_detector);
                if(*can_go){
                    go_left(map, map_row, map_column, player_current_row, player_current_column);
                }
                print_state(*player_blood, *player_strength, *player_money);
                print_map(map, map_row, map_column, mine_detector);
                *can_go = false;
                flag = false;
                check_die_or_win(map, map_row, map_column, player_blood, *total_monster, play, totem_of_undying);
                break;
            case 9:
                printf("Bye bye! Have a nice day.\n");
                *play = false;
                flag = false;
                break;
            default:
                printf("\033[40;31mInvalid input! Input again.\033[0m\n");
                flag = false;
                break;
        }
        setup_gun(map, map_row, map_column, gun_exist);
    }
}

void check_die_or_win(int **map,int map_row,int map_column,int *player_blood,int total_monster, bool *play, bool *totem_of_undying){
    int flag = 0;
    for (int i = 0; i < map_row;i++){
        for (int j = 0; j < map_column;j++){
            if(map[i][j]==5||map[i][j]==6||map[i][j]==7||map[i][j]==8){
                flag++;
            }
        }
    }

        if (*player_blood <= 0){
            if (*totem_of_undying){
                printf("You just escaped from the Grim Reaper\n");
                *player_blood = 10;
            }
            else{
        printf("Oh o! You GG~\n");
        printf("Have a nice day!\n");
        *play = false;
        }
    }
    if(flag==0){
        printf("You win!!\n");
        printf("Have a nice day!\n");
        *play = false;
    }
}

void print_state(int player_blood, int player_strength, int player_money){
    printf("HP: %d      ATK: %d     Coin: $%d\n", player_blood, player_strength, player_money);
}