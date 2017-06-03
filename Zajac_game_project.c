#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <conio.h>
#include <string.h>
#include <math.h>
typedef enum {FALSE=0, TRUE=1} bool;
enum {WEAPON, SHIELD, ARMOR, PIKEMEN=0, ARCHERS, BARD, SHAMAN, DWARVEN=0, ELVEN, DRAGON, ALLIANCE_ARMY};
enum {SHOP=0, QUEST, TRAVEL, WATCHTOWER, CHAPEL, FORGE, DRAGON_LAIR, TAVERN, ROYAL_HALL, SIEGE};

char option[40], cond[5]; //global variables

//structures

struct equipment
{
       char name[30];
       int lvl;
       int cost;
       char damage[5];
       int armor;
};

struct unit
{
       char name[20];
       int health;
       int maxhealth;
       int strength;
       int agility;
       char damage[5];
       int number;
};

struct acc
{
       char login[20];
       char pass[20];
};

struct character 
{
       char name[20];
       int health;
       int charisma;
       int diplomacy;
       int strength;
       int agility;
       int lvl;
       int exp;
       int gold;
       int quest[10];
       struct equipment equipment[3];//{weapon, shield, armor}
       int army[4];//{pikemen, archers, bard, shaman}
       bool pact[4];//{dwarven, elven, dragon, alliance_army;}
       char place[20];
};

//function declarations

void gamemenu(struct character);
struct character game(struct character);
int place(char[]);
struct character quest(struct character);
struct character travel(struct character);
int battle(struct character);
bool check(struct acc);
void create(struct acc);
struct character load(char *);
struct character save(struct character);
struct character levelup(struct character);
struct unit dmgtk(struct unit, int);
void card(struct character);
void help(void);
int diceroll(char[]);
int level(int);
char * redname(char[]);
char * upper(char[]);

//main function

int main()
{
  int op, i=0,cc;
  char c;
  struct acc account;
  struct character player;
  srand(time(NULL));
  do
  {
      system("cls");
      fflush(stdin);
      printf("Main menu:\n");
      printf("1. LOG in.\n2. CREATE new character.\n3. QUIT.\n");
      gets(option);
      if((strcmp(upper(option),"LOG")==0)||(strcmp(upper(option),"1")==0)) op=1;
      else if((strcmp(upper(option),"CREATE")==0)||(strcmp(upper(option),"2")==0)) op=2;
      else if((strcmp(upper(option),"QUIT")==0)||(strcmp(upper(option),"3")==0)) op=0;
      else op=-1;
      
      switch (op)
      {
             case 0: break; //quit
             case 1:                //logging
             {
                  system("cls");
                  printf("Login: ");
                  gets(account.login);
                  printf("Password: ");
                  gets(account.pass);
                  if(check(account))
                      {
                          printf("Congratulations, you have successfully logged in.\n");
                          getch();
                          player=load(account.login);
                          gamemenu(player);
                      }
                  else {printf("Oops! That's the wrong login or password.\n"); getch();}
             } break;
             case 2:                 //creating new character
             {
                  system("cls");
                  printf("Enter your name (which will be your login also): ");
                  gets(account.login);
                  printf("Enter your password: ");
                  fflush(stdin);
                  gets(account.pass);
                  create(account);
             } break;
             default:
             {
                     printf("This is not a proper function.\n");
                     getch();
             }
      }
  } while(op!=0); //quit
  return 0;
}

//functions

void gamemenu(struct character player)
{
     int op, i;
     do
     {
          system("cls");
          printf("Tip: while playing you can type 'card' to open your character card.\n");
          printf("Logged as %s.\n", player.name);
          if (player.exp==-1)
          {
              printf("Game menu:\n1. NEW game.\n2. HELP.\n3. EXIT.\n");
              gets(option);
              if((strcmp(upper(option),"NEW")==0)||(strcmp(upper(option),"1")==0)) op=2;
              else if((strcmp(upper(option),"EXIT")==0)||(strcmp(upper(option),"3")==0)) op=0;
              else if((strcmp(upper(option),"HELP")==0)||(strcmp(upper(option),"2")==0)) {help(); op=5;}
              else op=-1;
          }
          else
          {
              printf("Game menu:\n1. CONTINUE game.\n2. NEW game.\n3. SAVE game.\n4. LOAD game.\n5. HELP.\n6. EXIT.\n");
              gets(option);
              if((strcmp(upper(option),"CONTINUE")==0)||(strcmp(upper(option),"1")==0)) op=1;
              else if((strcmp(upper(option),"NEW")==0)||(strcmp(upper(option),"2")==0)) 
              {
                   printf("Are you sure, you want start a new game? You won't be able to run this save anymore.\n");
                   gets(cond);
                   if((strcmp(upper(cond),"YES")==0)||(strcmp(upper(cond),"Y")==0)) op=2;
                   else if((strcmp(upper(cond),"NO")==0)||(strcmp(upper(cond),"N")==0)) op=5;
                   else op=-1;
              }
              else if((strcmp(upper(option),"SAVE")==0)||(strcmp(upper(option),"3")==0)) op=3;
              else if((strcmp(upper(option),"LOAD")==0)||(strcmp(upper(option),"4")==0)) op=4;
              else if((strcmp(upper(option),"EXIT")==0)||(strcmp(upper(option),"6")==0)) op=0;
              else if((strcmp(upper(option),"HELP")==0)||(strcmp(upper(option),"5")==0)) {help(); op=5;}
              else if(strcmp(upper(option),"CARD")==0) {card(player); op=5;}
              else op=-1;
          }
          system("cls");
          switch (op)
          {
                 case 0: break; //exit
                 case 1: //continue
                 {
                      player=game(player);
                      save(player);
                 } break;
                 case 2: //new game
                 {
                      player.exp=0;
                      player.lvl=level(player.exp);
                      player.health=100;
                      player.charisma=10;
                      player.diplomacy=0;
                      player.strength=1;
                      player.agility=1;
                      player.gold=diceroll("4d4")*10;
                      sprintf(player.equipment[WEAPON].name, "0");
                      sprintf(player.equipment[WEAPON].damage, "1d1");
                      sprintf(player.equipment[ARMOR].name, "0");
                      sprintf(player.equipment[SHIELD].name, "0");
                      player.equipment[ARMOR].armor=player.equipment[SHIELD].armor=0;
                      for(i=0; i<4; i++)
                      {
                               player.army[i]=0;
                               player.pact[i]=0;
                      }
                      for (i=0;i<10;i++) player.quest[i]=-1;
                      sprintf(player.place, "Ariador"); //player.place="Ariador"; why not workin?
                      save(player);
                      player=game(player);
                      save(player);
                 } break;
                 case 3: //save game
                 {
                      save(player);
                 } break;
                 case 4: //load game
                 {
                      load(player.name);
                 } break;
                 case 5: break; //no action
                 default:
                 {
                         printf("This is not a proper function.\n");
                         getch();
                 } break;
          }
     } while(op!=0); //exit
     return;
}

struct character game(struct character player)  //GAME
{
     struct equipment weapon[20], armor[15], shield[10];
     char temp[20], option1[10], option2[10];
     int op,count;
     FILE *f;
     while(TRUE)
     {
          system("cls");
          if(player.quest[9]==1) //player had just win the game
          {
              printf("You have just win the game, thanks for playing.\n");
              getch();
              break;
          }
          printf("%s lvl: %d hp: %d charisma: %d gold: %d\n", player.name, player.lvl, player.health, player.charisma, player.gold);
          if (player.exp==0) printf("You wake up in some kind of slums in Ariador. Though you feel rested,\nyour clothes are nothing more than worn out pieces of cloth and broken bone is\nthe only thing you can call 'weapon'. Fortunatelly you find some gold coins\nso you can buy some equipment.\n");
          op=place(player.place);
          if (op==27) break;
          else switch(op)
          {
               case -1: card(player); break; //opening character card
               case SHOP:  //Shopping
               {
                    while(TRUE)
                    {
                       system("cls");
                       printf("%s lvl: %d hp: %d charisma: %d gold: %d\n", player.name, player.lvl, player.health, player.charisma, player.gold);
                       printf("\nStore:\n");
                       f=fopen("shop\\weapon.txt", "rt");
                       printf("\nWeapons:\n");
                       count=0;
                       while(!feof(f)) //receiving the weapon list
                       {
                             fscanf(f, "%d %s %s %d", &weapon[count].lvl, weapon[count].name, weapon[count].damage, &weapon[count].cost);
                             redname(weapon[count].name);
                             if(player.lvl>=weapon[count].lvl) printf("%d. %s, damage:%s cost:%d gold\n",count+1, weapon[count].name, weapon[count].damage, weapon[count].cost);
                             count++;
                       }
                       fclose(f);
                       f=fopen("shop\\armor.txt", "rt");
                       printf("\nArmors:\n");
                       count=0;
                       while(!feof(f)) //receiving the armor list
                       {
                             fscanf(f, "%d %s %d %d", &armor[count].lvl, armor[count].name, &armor[count].armor, &armor[count].cost);
                             redname(armor[count].name);
                             if(player.lvl>=armor[count].lvl) printf("%d. %s, armor:%d cost:%d gold\n",count+1, armor[count].name, armor[count].armor, armor[count].cost);
                             count++;
                       }
                       fclose(f);
                       f=fopen("shop\\shield.txt", "rt");
                       printf("\nShields:\n");
                       count=0;
                       while(!feof(f)) //receiving the shield list
                       {
                             fscanf(f, "%d %s %d %d", &shield[count].lvl, shield[count].name, &shield[count].armor, &shield[count].cost);
                             redname(shield[count].name);
                             if(player.lvl>=shield[count].lvl) printf("%d. %s, armor:%d cost:%d gold\n",count+1, shield[count].name, shield[count].armor, shield[count].cost);
                             count++;
                       }
                       fclose(f);
                       printf("\nWould you like to buy something?\nBACK to exit.\n");
                       fflush(stdin);
                       gets(option);
                       if(strcmp(upper(option),"BACK")==0)break;
                       else if(strcmp(upper(option),"CARD")==0) card(player);
                       else if(strcmp(upper(option),"ROBINHOOD")==0)
                       {
                            if (player.gold<500)
                            {
                               printf("Cheater!!!");
                               player.gold=500;
                               save(player);
                               getch();
                            }
                            
                       }
                       else for(count=0; count<20; count++)
                       {
                             sprintf(temp, "%s", weapon[count].name);
                             if(strcmp(upper(option),upper(temp))==0) //if the chosen thing is weapon
                                   {
                                           printf("Do you really want to buy %s?\n", weapon[count].name);
                                           gets(cond);
                                           if((strcmp(upper(cond),"YES")==0)||(strcmp(upper(cond),"Y")==0))
                                           {
                                                  if (player.exp==0) player.exp=1;
                                                  player.equipment[WEAPON]=weapon[count];
                                                  player.gold=player.gold-weapon[count].cost;
                                                  break;
                                           }
                                           else if((strcmp(upper(cond),"NO")==0)||(strcmp(upper(cond),"N")==0))break;
                                   }
                             else if (count<15) //or armor
                             {
                                 sprintf(temp, "%s", armor[count].name);
                                 if (strcmp(upper(option),upper(temp))==0)
                                 {
                                           printf("Do you really want to buy %s?\n", armor[count].name);
                                           gets(cond);
                                           if((strcmp(upper(cond),"YES")==0)||(strcmp(upper(cond),"Y")==0))
                                           {
                                                  if (player.exp==0) player.exp=1;
                                                  player.equipment[ARMOR]=armor[count];
                                                  player.gold=player.gold-armor[count].cost;
                                                  break;
                                           }
                                           else if((strcmp(upper(cond),"NO")==0)||(strcmp(upper(cond),"N")==0))break;
                                   }
                             }
                             if (count<10) // or shield
                             {
                                 sprintf(temp, "%s", shield[count].name);
                                 
                                 if (strcmp(upper(option),upper(temp))==0)
                                 {
                                           printf("Do you really want to buy %s?\n", shield[count].name);
                                           gets(cond);
                                           if((strcmp(upper(cond),"YES")==0)||(strcmp(upper(cond),"Y")==0))
                                           {
                                                  player.equipment[SHIELD]=shield[count];
                                                  player.gold=player.gold-shield[count].cost;
                                                  break;
                                           }
                                           else if((strcmp(upper(cond),"NO")==0)||(strcmp(upper(cond),"N")==0))break;
                                   }
                             }
                       }
                       save(player);
                    }
               } break;
               case QUEST: //Quest menu
               {
                    player=quest(player);
                    save(player);
               } break;
               case TRAVEL: //Travelling
               {
                    player=travel(player);
                    save(player);
                    getch();
               } break;
               case WATCHTOWER: //Recruiting Pikemen
               {
                    fflush(stdin);
                    system("cls");
                    printf("%s lvl: %d hp: %d charisma: %d gold: %d\n", player.name, player.lvl, player.health, player.charisma, player.gold);
                    if (player.lvl<5) {printf("Guards laugh at you.\n\"Are you serious? You want to come in? What for, you dirty rogue?\nGet off and step back where'd you come from.\nNOW!!!\"\n\nYou cannot visit this place until level 5.\n");getch();}
                    else
                    {
                        printf("Guards look at you with high respect.\nDo you want to recruit Pikemen?\n");
                        gets(cond);
                        if((strcmp(upper(cond),"YES")==0)||(strcmp(upper(cond),"Y")==0))
                        {
                              printf("How many?(10 gold per unit, 1 charisma point)\n");
                              scanf("%d", &count);
                              if (player.charisma>=count) //if player has enough charisma points for recruiting
                              {
                                   if (player.gold>=10*count)
                                   {
                                      player.army[PIKEMEN]=player.army[PIKEMEN]+count;
                                      player.charisma=player.charisma-count;
                                      player.gold=player.gold-10*count;
                                      save(player);
                                   }
                                   else {printf("You don't have enough gold.\n"); getch();}
                              }
                              else {printf("You don't have enough charisma points.\n"); getch();}
                        }
                        else if((strcmp(upper(cond),"NO")==0)||(strcmp(upper(cond),"N")==0));
                    }
               } break;
               case CHAPEL:  //Obtaining Dwarven pact
               {
                    while(TRUE)
                    {
                         system("cls");
                         printf("%s lvl: %d hp: %d charisma: %d gold: %d\n", player.name, player.lvl, player.health, player.charisma, player.gold);
                         printf("\nThe dwarven chapel is amazingly huge and rich. You hear choir and prayers from all around you as echo drifts the sounds.\nThere is one Royal Priest standing in the centre.\n");
                         printf("1. Come along the royal PRIEST.\n2. EXIT the chapel.\n");
                         gets(option);
                         if((strcmp(upper(option),"PRIEST")==0)||(strcmp(upper(option),"1")==0))
                         {
                                 if (player.pact[DWARVEN]==0)
                                 {
                                       if(player.diplomacy>0)
                                       {
                                            printf("\"Welcome, my son. You say, you are fighting against the evil and cruelty of this world? So do we. Would you accept our blessings?\"\n");
                                            gets(cond);
                                            if((strcmp(upper(cond),"YES")==0)||(strcmp(upper(cond),"Y")==0))
                                            {
                                                  player.pact[DWARVEN]=1;
                                                  save(player);
                                            }
                                       }
                                       else {printf("\"Are you in need, my son? Do you need an enlightment?\"\nYou feel like negotiations were not satisfying. Perhaps you should develop your diplomacy?\n"); getch();}
                                 }
                                 else {printf("It is nice to see you, my son. Unfortunatelly, we have done everything we could to help you.\n"); getch();}
                         }
                         else if (strcmp(upper(option),"CARD")==0) card(player);
                         else if((strcmp(upper(option),"EXIT")==0)||(strcmp(upper(option),"2")==0)) break;
                    }
               } break;
               case FORGE:  //Obtaining Elven pact, recruiting Archers
               {
                    while(TRUE)
                    {
                         system("cls");
                         printf("%s lvl: %d hp: %d charisma: %d gold: %d\n", player.name, player.lvl, player.health, player.charisma, player.gold);
                         printf("\nThe heat of ovens can be felt from where you stand. Next to the forge many elves are training with their bows. The one with mighty sword, simply the leader, is looking at you.\n");
                         if(player.pact[ELVEN]==1)
                         {
                                 printf("1. Speak with LEADER.\n2. Recruit ARCHERS.\n3. EXIT.\n");
                                 gets(option);
                                 if((strcmp(upper(option),"LEADER")==0)||(strcmp(upper(option),"1")==0))
                                 {
                                      printf("\"I think there's nothing I can do to you. Good luck on track!\"\n");
                                      getch();
                                 }
                                 else if((strcmp(upper(option),"ARCHERS")==0)||(strcmp(upper(option),"2")==0))
                                 {
                                      printf("Archers salute as you step closer.\nDo you want to recruit Archers?\n");
                                      gets(cond);
                                      if((strcmp(upper(cond),"YES")==0)||(strcmp(upper(cond),"Y")==0))
                                      {
                                            printf("How many?(20 gold per unit, 2 charisma point)\n");
                                            scanf("%d", &count);
                                            if (player.charisma>=2*count) //if player has enough charisma points for recruiting
                                            {
                                                if (player.gold>=20*count)
                                                {
                                                      player.army[ARCHERS]=player.army[ARCHERS]+count;
                                                      player.charisma=player.charisma-2*count;
                                                      player.gold=player.gold-20*count;
                                                      save(player);
                                                }
                                                else {printf("You don't have enough gold.\n"); getch();}
                                            }
                                            else {printf("You don't have enough charisma points.\n"); getch();}
                                      }
                                      else if((strcmp(upper(cond),"NO")==0)||(strcmp(upper(cond),"N")==0));
                                 }
                                 else if (strcmp(upper(option),"CARD")==0) card(player);
                                 else if((strcmp(upper(option),"EXIT")==0)||(strcmp(upper(option),"3")==0)) break;
                         }
                         else
                         {
                                 printf("1. Speak with LEADER.\n2. EXIT.\n");
                                 gets(option);
                                 if((strcmp(upper(option),"LEADER")==0)||(strcmp(upper(option),"1")==0))
                                 {
                                      if(player.pact[DWARVEN]==1)
                                      {
                                           if(player.diplomacy>1)
                                           {
                                                printf("\"Oh, so you might be that hero I heard of. The priests of Kharamdoor told me that you're on holy war or something... Anyway, I like it! Will you accept my enchantment on your weapon? Your support is welcomed. A pocket of 50 gold coins will do. So?\"\n");
                                                gets(cond);
                                                if((strcmp(upper(cond),"YES")==0)||(strcmp(upper(cond),"Y")==0))
                                                {
                                                      if(player.gold>=50)
                                                      {
                                                           printf("\"That's what I'm talking about! Aha, and be sure that my archers would be on your call.\"\n");
                                                           player.gold=player.gold-50;
                                                           player.pact[ELVEN]=1;
                                                           save(player);
                                                           getch();
                                                      }
                                                      else {printf("\"What is that?! I told you that I need money. No money, no service, sorry.\"\n"); getch();}
                                                }
                                           }
                                           else {printf("\"Hey, you're the guy that priests told me about? But you don't look so magnificent as from their tales. Go and proove yourself worthy. We're done for now!\"\nIt seems as your diplomacy failed, you need to develop it first.\n"); getch();}
                                      }
                                      else {printf("\"Who are you and what brings you here? You want me to help you? Ha! I don't trust you. Maybe you should beg dwarven priests in the Chapel of Kharamdoor... You're still here?\"\n"); getch();}
                                 }
                                 else if (strcmp(upper(option),"CARD")==0) card(player);
                                 else if((strcmp(upper(option),"EXIT")==0)||(strcmp(upper(option),"2")==0)) break;
                         }
                    }
               } break;
               case DRAGON_LAIR:  //Obtaining Dragon pact
               {
                    while(TRUE)
                    {
                          system("cls");
                          printf("%s lvl: %d hp: %d charisma: %d gold: %d\n", player.name, player.lvl, player.health, player.charisma, player.gold);
                          printf("\nThe odor of sulfur is very intense here. You feel the existance of dragon, though you can't see it yet. The creature must be sleeping.\n1. Wake up DRAGON.\n2. Move BACK silently.\n");
                          gets(option);
                          if((strcmp(upper(option),"DRAGON")==0)||(strcmp(upper(option),"1")==0))  //Fighting dragon 
                          {
                                  if((player.quest[5]!=0)&&(player.quest[5]!=1)) player.quest[5]=0;
                                  else {printf("You possibly have just beaten up the dragon, and now it is on your service. You shouldn't attack it as it is your ally.\n"); getch();}
                                  if((player.quest[5]==0)&&(battle(player)==1))
                                  {
                                           player=load(player.name);
                                           printf("Congratulations! You have beaten up the dragon, which is now on your service.\n");
                                           player.pact[DRAGON]=1;
                                           player.quest[5]=1;
                                           save(player);
                                           getch();
                                  }
                                  else {player=load(player.name); player.quest[5]=-1;}
                          }
                          else if (strcmp(upper(option),"CARD")==0) card(player);
                          else if((strcmp(upper(option),"BACK")==0)||(strcmp(upper(option),"2")==0)) break;
                    }
               } break;
               case TAVERN:  //Recruiting Bard and Shaman, buying meals
               {
                    while(TRUE)
                    {
                             system("cls");
                             printf("%s lvl: %d hp: %d charisma: %d gold: %d\n", player.name, player.lvl, player.health, player.charisma, player.gold);
                             printf("It's warm in here. You can relax for some time, or buy meal that will heal your wounds.\n1. Buy MEAL.\n");
                             if(strcmp(upper(player.place), "BELLASEE FALLS")==0)
                             {
                                        printf("2. Ask for BARD.\n3. Ask for SHAMAN.\n4. EXIT.\n");
                                        gets(option);
                                        if((strcmp(upper(option),"MEAL")==0)||(strcmp(upper(option),"1")==0))
                                        {
                                             printf("Would you like to buy meal for 1 gold?\n");
                                             gets(cond);
                                             if((strcmp(upper(cond),"YES")==0)||(strcmp(upper(cond),"Y")==0))
                                             {
                                                   if(player.gold>=1)
                                                   {
                                                        player.gold--;
                                                        player.health=90+10*player.lvl;
                                                        save(player);
                                                   }
                                                   else {printf("You don't have enough gold.\n"); getch();}
                                             };
                                        }
                                        else if((strcmp(upper(option),"BARD")==0)||(strcmp(upper(option),"2")==0))
                                        {
                                             if (player.pact[ALLIANCE_ARMY]==1)
                                             {
                                                   if (player.army[BARD]==1) {printf("You can have only one Bard in your army, recruiting another one won't change anything.\n"); getch();}
                                                   else
                                                   {
                                                       printf("Would you like to recruit Bard (50 gold, 5 charisma points) to your army?\n");
                                                       gets(cond);
                                                       if((strcmp(upper(cond),"YES")==0)||(strcmp(upper(cond),"Y")==0))
                                                       {
                                                             if (player.charisma>=5) //if player has enough charisma points for recruiting
                                                             {
                                                                if (player.gold>=50)
                                                                {
                                                                   player.army[BARD]=1;
                                                                   player.charisma=player.charisma-5;
                                                                   player.gold=player.gold-50;
                                                                   save(player);
                                                                }
                                                                else {printf("You don't have enough gold.\n"); getch();}
                                                             }
                                                             else {printf("You don't have enough charisma points.\n"); getch();}
                                                       }
                                                   }
                                             }
                                             else {printf("People look at you suspiciously. It seems you should gain some pact with them.\n"); getch();}
                                        }
                                        else if((strcmp(upper(option),"SHAMAN")==0)||(strcmp(upper(option),"3")==0))
                                        {
                                             if (player.pact[ALLIANCE_ARMY]==1)
                                             {
                                                 if (player.army[SHAMAN]==1) {printf("You can have only one Shaman in your army, recruiting another one won't change anything.\n"); getch();}
                                                   else
                                                   {
                                                       printf("Would you like to recruit Shaman (50 gold, 5 charisma points) to your army?\n");
                                                       gets(cond);
                                                       if((strcmp(upper(cond),"YES")==0)||(strcmp(upper(cond),"Y")==0))
                                                       {
                                                             if (player.charisma>=5) //if player has enough charisma points for recruiting
                                                             {
                                                                if (player.gold>=50)
                                                                {
                                                                   player.army[SHAMAN]=1;
                                                                   player.charisma=player.charisma-5;
                                                                   player.gold=player.gold-50;
                                                                   save(player);
                                                                }
                                                                else {printf("You don't have enough gold.\n"); getch();}
                                                             }
                                                             else {printf("You don't have enough charisma points.\n"); getch();}
                                                       }
                                                   }
                                             }
                                             else {printf("People look at you suspiciously. It seems you should gain some pact with them.\n"); getch();}
                                        }
                                        else if (strcmp(upper(option),"CARD")==0) card(player);
                                        else if((strcmp(upper(option),"EXIT")==0)||(strcmp(upper(option),"4")==0)) break;
                             }
                             else
                             {
                                        printf("2. EXIT.\n");
                                        gets(option);
                                        if((strcmp(upper(option),"MEAL")==0)||(strcmp(upper(option),"1")==0))
                                        {
                                              printf("Would you like to buy meal for 1 gold?\n");
                                              gets(cond);
                                              if((strcmp(upper(cond),"YES")==0)||(strcmp(upper(cond),"Y")==0))
                                              {
                                                   if(player.gold>=1)
                                                   {
                                                        player.gold--;
                                                        player.health=90+10*player.lvl;
                                                        save(player);
                                                   }
                                                   else {printf("You don't have enough gold.\n"); getch();}
                                              };
                                        }
                                        else if (strcmp(upper(option),"CARD")==0) card(player);
                                        else if((strcmp(upper(option),"EXIT")==0)||(strcmp(upper(option),"2")==0)) break;
                             }
                    }
               } break;
               case ROYAL_HALL:  //obtaining the Alliance army pact
               {
                    while(TRUE)
                    {
                             system("cls");
                             printf("%s lvl: %d hp: %d charisma: %d gold: %d\n", player.name, player.lvl, player.health, player.charisma, player.gold);
                             printf("\nThe royal hall as the whole castle in its mighty view is breathtaking. What the power should have the one to conquer it all?... It is better to be ally with that king.\n");
                             printf("1. Speak to the KING.\n2. EXIT the castle.\n");
                             gets(option);
                             if((strcmp(upper(option),"KING")==0)||(strcmp(upper(option),"1")==0))
                             {
                                     if(player.pact[DRAGON]==1)
                                     {
                                             printf("When you go closer to the king, the royal guards block your way. But king's word allows you to pass through the gate made from halberds as he wants to speak to you.\n\"So you are that young hero who has beaten the dragon. Be sure that I appreciate your action.\"\n");
                                             if ((player.diplomacy==4)&&(player.gold>=100))
                                             {
                                                    printf("1. Thank king and offer your GIFT (100 gold coins).\n2. Thank king and go OUT.\n");
                                                    gets(cond);
                                                    if((strcmp(upper(cond),"GIFT")==0)||(strcmp(upper(cond),"1")==0))
                                                    {
                                                             player.gold=player.gold-100;
                                                             player.pact[ALLIANCE_ARMY]=1;
                                                             player.charisma=player.charisma+100;
                                                             printf("\"Oh, you are so generous. There will be Bard and Shaman on the tavern ready for your service. Now, be gone.\"\n");
                                                             getch();
                                                    }
                                                    else if((strcmp(upper(cond),"OUT")==0)||(strcmp(upper(option),"2")==0)) break;
                                                    else {printf("Not a proper function.\n"); getch();}
                                             }
                                             else getch();
                                     }
                                     else {printf("When you go closer to the king, the royal guards block your way. It seems they don't know about you yet. Perhaps duelling that dragon in Isnet'Pa would earn you some reputation?...\n"); getch();}
                             }
                             else if (strcmp(upper(option),"CARD")==0) card(player);
                             else if((strcmp(upper(option),"EXIT")==0)||(strcmp(upper(option),"2")==0)) break;
                    }
               } break;
               case SIEGE: //siege the castle of Limestone Ridge to win the game
               {
                    player.quest[9]=0;
                    if(battle(player)==1)
                    {
                       player=load(player.name);
                       player.quest[9]=1;
                       save(player);
                       printf("Congratulations! You win the game!\nYou rocks!!!\n");
                       getch();
                    }
                    else {player=load(player.name); player.quest[9]=-1; save(player);}
               } break;
               default:
               {
                       printf("This is not a proper function.\n");
                       getch();
               } break;
          }
     }
     return player;
}

int place (char city[])
{
    fflush(stdin);
    if (strcmp(upper(city), "LIMESTONE RIDGE")==0)
    {
        printf("1. SIEGE the castle of Limestone Ridge.\n2. TRAVEL to another city.\n3. EXIT.\n");
        gets(option);
        
        if((strcmp(upper(option),"SIEGE")==0)||(strcmp(upper(option),"1")==0)) return SIEGE;
        else if((strcmp(upper(option),"TRAVEL")==0)||(strcmp(upper(option),"2")==0)) return TRAVEL;
        else if((strcmp(upper(option),"EXIT")==0)||(strcmp(upper(option),"3")==0)) return 27;
        else if (strcmp(upper(option),"CARD")==0) return -1;
        else return -10;
        
    }
    else
    {
        printf("1. Go to SHOP.\n2. QUEST advertisements.\n3. TRAVEL to another city.\n4. TAVERN.\n");
        if (strcmp(upper(city), "ARIADOR")==0) printf("5. WATCHTOWER.\n");
        else if (strcmp(upper(city), "KHARAMDOOR")==0) printf("5. CHAPEL.\n");
        else if (strcmp(upper(city), "ELVENMORT")==0) printf("5. FORGE.\n");
        else if (strcmp(upper(city), "ISNET'PA")==0) printf("5. DRAGON lair.\n");
        else if (strcmp(upper(city), "BELLASEE FALLS")==0) printf("5. ROYAL hall.\n");
        printf("6. EXIT.\n");
    
        gets(option);
    
        if((strcmp(upper(option),"SHOP")==0)||(strcmp(upper(option),"1")==0)) return SHOP;
        else if((strcmp(upper(option),"QUEST")==0)||(strcmp(upper(option),"2")==0)) return QUEST;
        else if((strcmp(upper(option),"TRAVEL")==0)||(strcmp(upper(option),"3")==0)) return TRAVEL;
        else if((strcmp(upper(option),"TAVERN")==0)||(strcmp(upper(option),"4")==0)) return TAVERN;
        else if((strcmp(upper(option),"EXIT")==0)||(strcmp(upper(option),"6")==0)) return 27;
        else if ((strcmp(upper(city), "ARIADOR")==0)&&((strcmp(upper(option),"WATCHTOWER")==0)||(strcmp(upper(option),"5")==0))) return WATCHTOWER;
        else if ((strcmp(upper(city), "KHARAMDOOR")==0)&&((strcmp(upper(option),"CHAPEL")==0)||(strcmp(upper(option),"5")==0))) return CHAPEL;
        else if ((strcmp(upper(city), "ELVENMORT")==0)&&((strcmp(upper(option),"FORGE")==0)||(strcmp(upper(option),"5")==0))) return FORGE;
        else if ((strcmp(upper(city), "ISNET'PA")==0)&&((strcmp(upper(option),"DRAGON")==0)||(strcmp(upper(option),"5")==0))) return DRAGON_LAIR;
        else if ((strcmp(upper(city), "BELLASEE FALLS")==0)&&((strcmp(upper(option),"ROYAL")==0)||(strcmp(upper(option),"5")==0))) return ROYAL_HALL;
        else if (strcmp(upper(option),"CARD")==0) return -1;
        else return -10;
    }
}

struct character quest (struct character player) //quest menu
{
    char choice[10];
    int treasure;
    while(TRUE)
    {
       system("cls");
       printf("%s lvl: %d hp: %d charisma: %d gold: %d\n", player.name, player.lvl, player.health, player.charisma, player.gold);
       printf("You stand beside the quest board. There is a dungeon entrance next to it.\nWhat do you do?\n1. Look at quest BOARD.\n2. Go into the DUNGEON.\n3. Go BACK.\n");
       gets(option);
       if ((strcmp(upper(option),"BOARD")==0)||(strcmp(upper(option),"1")==0))
       {
            system("cls");
            printf("You look at the quest board:\n");
            if (player.lvl==1) //1st lvl quest
            {
                  if (player.quest[0]<0) //starting quest
                  {
                      printf("A volunteer needed to investigate chambers in dungeon and finding out the forgotten documents.\n");
                      printf("Accept that quest?\n");
                      gets(cond);
                      if ((strcmp(upper(cond),"YES")==0)||(strcmp(upper(cond),"Y")==0))
                           player.quest[0]=0;
                  }
                  else if (player.quest[0]==1) //if done
                  {
                      printf("Would you like to inform about your success and get your reward?\n");
                      gets(cond);
                      if ((strcmp(upper(cond),"YES")==0)||(strcmp(upper(cond),"Y")==0))
                      {
                           printf("You gain 10 gold and 99 exp.\n");
                           player.quest[0]=2;
                           player.gold=player.gold+10;
                           player.exp=player.exp+99;
                           player=levelup(player);
                           getch();
                      }
                  }
                  else if (player.quest[0]==0){printf("A volunteer needed to investigate chambers in dungeon and finding out the forgotten documents.\n"); printf("You did not found the forgotten documents yet.\n"); getch();} //if not completed
                  else {printf("There is no quest available right now.\n"); getch();}
            }
            else if(player.lvl==2) //2nd lvl quest
            {
                  if (player.quest[1]<0) //starting quest
                  {
                      printf("There are Villains and Thieves dangering the dungeon. Slay 5 of them!\n");
                      printf("Accept that quest?\n");
                      gets(cond);
                      if ((strcmp(upper(cond),"YES")==0)||(strcmp(upper(cond),"Y")==0))
                           player.quest[1]=0;
                  }
                  else if (player.quest[1]==5) //if done
                  {
                      while(TRUE)
                      {
                           printf("Would you like to inform about your success and get your reward?\n");
                           gets(cond);
                           if ((strcmp(upper(cond),"YES")==0)||(strcmp(upper(cond),"Y")==0))
                           {
                                  printf("You gain 50 gold and 150 exp.\n");
                                  player.quest[1]=6;
                                  player.gold=player.gold+50;
                                  player.exp=player.exp+150;
                                  player=levelup(player);
                                  getch();
                                  break;
                           }
                           else if ((strcmp(upper(cond),"NO")==0)||(strcmp(upper(cond),"N")==0)) break;
                      }
                  }
                  else if ((player.quest[1]>=0)&&(player.quest[1]<5)) {printf("There are Villains and Thieves dangering the dungeon. Slay 5 of them!\n"); printf("You haven't slain 5 Villains or Thieves yet. %d/5 slain.\n", player.quest[1]); getch();} //if not completed
                  else {printf("There is no quest available right now.\n"); getch();}
            }
            else if(player.lvl==3) //3rd lvl quest
            {
                  if (player.quest[2]<0) //starting quest
                  {
                      printf("The group of children had lost in dungeons. It's time to rescue them!\n");
                      printf("Accept that quest?\n");
                      gets(cond);
                      if ((strcmp(upper(cond),"YES")==0)||(strcmp(upper(cond),"Y")==0))
                           player.quest[2]=0;
                  }
                  else if (player.quest[2]==1) //if done
                  {
                      printf("Would you like to inform about your success and get your reward?\n");
                      gets(cond);
                      if ((strcmp(upper(cond),"YES")==0)||(strcmp(upper(cond),"Y")==0))
                      {
                           printf("You gain 60 gold and 200 exp.\n");
                           player.quest[2]=2;
                           player.gold=player.gold+60;
                           player.exp=player.exp+200;
                           player=levelup(player);
                           getch();
                      }
                  }
                  else if(player.quest[2]==0) {printf("The group of children had lost in dungeons. It's time to rescue them!\n"); printf("You haven't found children yet.\n"); getch();} //if not completed
                  else {printf("There is no quest available right now.\n"); getch();}
            }
            else if ((player.lvl==4)||(player.lvl==5)) //4th lvl quest
            {
                  if (player.quest[3]<0) //starting quest
                  {
                      printf("Find out the smuggler's hideout somewhere in the dungeon.\n");
                      printf("Accept that quest?\n");
                      gets(cond);
                      if ((strcmp(upper(cond),"YES")==0)||(strcmp(upper(cond),"Y")==0))
                           player.quest[3]=0;
                  }
                  else if (player.quest[3]==1) //if done
                  {
                      printf("Would you like to inform about your success and get your reward?\n");
                      gets(cond);
                      if ((strcmp(upper(cond),"YES")==0)||(strcmp(upper(cond),"Y")==0))
                      {
                           printf("You gain 100 gold and 300 exp.\n");
                           player.quest[3]=2;
                           player.gold=player.gold+100;
                           player.exp=player.exp+300;
                           player=levelup(player);
                           getch();
                      }
                  }
                  else if(player.quest[3]==0) {printf("Find out the smuggler's hideout somewhere in the dungeon.\n"); printf("You haven't found children yet.\n"); getch();} //if not completed
                  else {printf("There is no quest available right now.\n"); getch();}
            }
            else {printf("You seems too strong. You should be on your way to defeat the evil of Limestone Ridge!\n"); getch();}
            
       }
       else if ((strcmp(upper(option),"DUNGEON")==0)||(strcmp(upper(option),"2")==0))
       {
            if((strcmp(upper(player.place),"ISNET'PA")==0)&&(player.pact[DRAGON]==0)) {printf("The dungeon entrance is burried by rocks. You wonder if dragon can reveal it for you...\n"); getch();}
            else while(TRUE)
            {
                system("cls");
                printf("%s lvl: %d hp: %d charisma: %d gold: %d\n", player.name, player.lvl, player.health, player.charisma, player.gold);
                printf("You are in a slimy dungeon. There seems to be some chambers.\n1. Enter FIRST chamber.\n2. Enter SECOND chamber.\n3. Enter THIRD chamber.\n4. Enter FOURTH chamber.\n5. Go OUT.\n");
                gets(choice);
                if ((strcmp(upper(choice),"FIRST")==0)||(strcmp(upper(choice),"1")==0))
                {
                     system("cls");
                     if (player.quest[3]==0)
                     {
                         printf("You see many packages scattered around. This must be the smugglers' hideout. You feel like someone is looking on you.\n");
                         getch();
                         if(battle(player))
                         {
                               player=load(player.name);
                               printf("You have done well. Now it's time to reveal the plans of smugglers' hideout. People are waiting.\n");
                               player.quest[3]=1;
                         }
                         else break;
                     }
                     else if(diceroll("1d10")<5)
                     {
                           if ((player.lvl>4)&&((player.army[PIKEMEN]==0)&&(player.army[ARCHERS]==0))) {printf("You cannot walk here alone! go and get some army.\n"); getch();}
                           else if ((player.quest[1]>=0)&&(player.quest[1]<5))
                           {
                                 if(battle(player))
                                 {
                                        player=load(player.name);
                                        player.quest[1]=player.quest[1]+1;
                                        printf("Quest: Villains and Thieves slain %d/5\n", player.quest[1]);
                                 }
                           }
                           else
                           {
                               battle(player);
                               player=load(player.name);
                           }
                     }
                     else printf("You find nothing.\n");
                     getch();
                }
                else if ((strcmp(upper(choice),"SECOND")==0)||(strcmp(upper(choice),"2")==0))
                {
                     system("cls");
                     if(diceroll("1d10")<5)
                     {
                           if ((player.lvl>4)&&((player.army[PIKEMEN]==0)&&(player.army[ARCHERS]==0))) {printf("You cannot walk here alone! go and get some army.\n"); getch();}
                           else if ((player.quest[1]>=0)&&(player.quest[1]<5))
                           {
                                 if(battle(player))
                                 {
                                        player=load(player.name);
                                        player.quest[1]=player.quest[1]+1;
                                        printf("Quest: Villains and Thieves slain %d/5\n", player.quest[1]);
                                 }
                           }
                           else
                           {
                               battle(player);
                               player=load(player.name);
                           }
                     }
                     else printf("You find nothing.\n");
                     getch();
                }
                else if ((strcmp(upper(choice),"THIRD")==0)||(strcmp(upper(choice),"3")==0))
                {
                     system("cls");
                     if (player.quest[2]==0)
                     {
                         printf("You see the group of lost children... But, wait, there's someone else!\n");
                         getch();
                         if(battle(player))
                         {
                               player=load(player.name);
                               printf("You luckily defended children. Now escort them back to quest board.\n");
                               player.quest[2]=1;
                         }
                         else break;
                     }
                     else if(diceroll("1d10")<5)
                     {
                           if ((player.lvl>4)&&((player.army[PIKEMEN]==0)&&(player.army[ARCHERS]==0))) {printf("You cannot walk here alone! go and get some army.\n"); getch();}
                           else if ((player.quest[1]>=0)&&(player.quest[1]<5))
                           {
                                 if(battle(player))
                                 {
                                        player=load(player.name);
                                        player.quest[1]=player.quest[1]+1;
                                        printf("Quest: Villains and Thieves slain %d/5\n", player.quest[1]);
                                 }
                           }
                           else
                           {
                               battle(player);
                               player=load(player.name);
                           }
                     }
                     else printf("You find nothing.\n");
                     getch();
                }
                else if ((strcmp(upper(choice),"FOURTH")==0)||(strcmp(upper(choice),"4")==0))
                {
                     system("cls");
                     if (player.quest[0]==0)
                     {
                         if(battle(player))
                         {
                               player=load(player.name);
                               while(TRUE)
                               {
                                    printf("You find a chest. Would you like to open it?\n");
                                    gets(cond);
                                    if ((strcmp(upper(cond),"YES")==0)||(strcmp(upper(cond),"Y")==0))
                                    {
                                         treasure=diceroll("1d4");
                                         printf("You find the forgotten documents and %d gold coins.", treasure);
                                         player.quest[0]=1;
                                         player.gold=player.gold+treasure;
                                         break;
                                    }
                                    else if ((strcmp(upper(cond),"NO")==0)||(strcmp(upper(cond),"N")==0)) break;
                               }
                         }
                         else break;
                     }
                     else if(diceroll("1d10")<5)
                     {
                           if ((player.lvl>4)&&((player.army[PIKEMEN]==0)&&(player.army[ARCHERS]==0))) {printf("You cannot walk here alone! go and get some army.\n"); getch();}
                           else if ((player.quest[1]>=0)&&(player.quest[1]<5))
                           {
                                 if(battle(player))
                                 {
                                        player=load(player.name);
                                        player.quest[1]=player.quest[1]+1;
                                        printf("Quest: Villains and Thieves slain %d/5\n", player.quest[1]);
                                 }
                           }
                           else
                           {
                               battle(player);
                               player=load(player.name);
                           }
                     }
                     else printf("You find nothing.\n");
                     getch();
                }
                else if ((strcmp(upper(choice),"OUT")==0)||(strcmp(upper(choice),"5")==0)) break;
                else if (strcmp(upper(choice),"CARD")==0) card(player);
                save(player);
            }
       }
       else if ((strcmp(upper(option),"BACK")==0)||(strcmp(upper(option),"3")==0)) break;
       else if (strcmp(upper(option),"CARD")==0) card(player);
       else if (strcmp(upper(option),"IWANTBEAKING")==0) if (player.exp<=1000) 
            {
                  player.exp=1000;
                  printf("CHEATER!!!");
                  player=levelup(player);
                  getch();
                  break;
            }
    }
    return player;
}

struct character travel(struct character player)  //travel menu
{
    int op,i,j=0,g[6], danger;
    char c[2],temp[6][20],playertown[20],towns[][20]={"ARIADOR","KHARAMDOOR","ELVENMORT","ISNET'PA","BELLASEE FALLS","LIMESTONE RIDGE"};
    sprintf(playertown, "%s", player.place);
    redname(playertown);
    for(i=0;i<6;i++)
    {
         if(strcmp(upper(towns[i]),upper(playertown))!=0) //excluding place where player already is
         {
               sprintf(temp[j], "%s",towns[i]);
               g[j]=i;
               j++;
         }
         else g[5]=i;
    }
    system("cls");
    if((player.army[PIKEMEN]==0)&&(player.army[ARCHERS]==0)) {printf("The roads are dangerous. You cannot travel alone.\n"); getch();}
    else do
    {
         
         system("cls");
         printf("Where would you like to travel:");
         for (i=0;i<5;i++) printf("\n%d. %s %d%% of danger", i+1, temp[i], abs(g[i]-g[5])*10);
         printf("?\nBACK to return.\n");
         gets(option);
         op=1;
         if (strcmp(upper(option),"BACK")==0) op=0;
         else if (strcmp(upper(option),"CARD")==0) card(player);
         else for(i=0;i<6;i++)
         {
              sprintf(c,"%d",i+1);
              if(strcmp(upper(option),upper(playertown))==0) printf("You are already here.\n");
              else if ((strcmp(upper(option),upper(temp[i]))==0)||(strcmp(upper(option),c)==0))
              {
                   printf("Are you sure?\n");
                   gets(cond);
                   if ((strcmp(upper(cond),"YES")==0)||(strcmp(upper(cond),"Y")==0))
                   {
                         danger=rand()%100-abs(g[i]-g[5])*10;   //checking for danger
                         if(danger<=40) //there is danger
                         {
                              if (battle(player)==1)
                              {
                                      player=load(player.name);
                                      sprintf(player.place, "%s", temp[i]);
                              }
                         }
                         else sprintf(player.place, "%s", temp[i]);
                         op=0;
                   }
                   else if ((strcmp(upper(cond),"NO")==0)||(strcmp(upper(cond),"N")==0))op=1;
              }
         }
    } while(op!=0);
    return player;
}

int battle(struct character player)
{
    int i, drag=1,result=-1, r, roll1, roll2, damage, round, provoke, song[4], curse[4], treasure, expgain;
    bool move[4];
    char select[10], plmove[10], choice[10], archersdamage[5];
    FILE *f;
    struct unit enemy[2], pikemen, archers, tempunit[6];
                                   //getting enemy
    if(player.lvl<5) r=rand()%player.lvl;
    else r=rand()%6;
    f=fopen("enemy.txt", "rt");
    i=0;
    while(!feof(f))
    {
          fscanf(f, "%s %d %d %d %s", tempunit[i].name, &tempunit[i].maxhealth, &tempunit[i].strength, &tempunit[i].agility, tempunit[i].damage);
          i++;
    }
    fclose(f);
    enemy[0]=tempunit[r];
    if(player.lvl<5) //only 1 enemy
    {
         enemy[0].number=1;
         enemy[0].health=enemy[0].maxhealth;
    }
    else //army
    {
         if(player.quest[5]==0)
         {
              sprintf(enemy[0].name, "Dragon");
              enemy[0].number=1;
              enemy[0].health=enemy[0].maxhealth=2000;
              enemy[0].strength=5;
              enemy[0].agility=5;
              sprintf(enemy[0].damage, "2d10");
              enemy[1].number=0;
              sprintf(enemy[1].name, " ");
              enemy[1].maxhealth=enemy[1].health=0;
         }
         else if (player.quest[9]==0)
         {
              sprintf(enemy[0].name, "Noble Knight");
              sprintf(enemy[1].name, "Grand Archer");
              enemy[0].number=enemy[1].number=75;
              enemy[0].maxhealth=enemy[0].health=enemy[1].maxhealth=enemy[1].health=150;
              enemy[0].strength=enemy[1].agility=5;
              enemy[0].agility=enemy[1].strength=4;
              sprintf(enemy[0].damage, "2d5");
              sprintf(enemy[1].damage, "1d10");
         }
         else
         {
              enemy[0].number=rand()%player.army[PIKEMEN]+2;
              enemy[0].health=enemy[0].maxhealth;
              if(diceroll("1d100")>50) //there is second line in enemy's army
              {
                      f=fopen("shooters.txt", "rt");
                      i=0;
                      while(!feof(f))
                      {
                                     fscanf(f, "%s %d %d %d %s", tempunit[i].name, &tempunit[i].maxhealth, &tempunit[i].strength, &tempunit[i].agility, tempunit[i].damage);
                                     i++;
                      }
                      fclose(f);
                      r=rand()%3;
                      enemy[1]=tempunit[r];
                      enemy[1].health=enemy[1].maxhealth;
                      enemy[1].number=rand()%player.army[ARCHERS]+2;
              }
              else //there is no second line enemy
              {
                   enemy[1].number=0;
                   sprintf(enemy[1].name, " ");
                   enemy[1].maxhealth=enemy[1].health=0;
              }
         }
    }

    if(player.lvl<5)  //fighting 1vs1
    {
        round=1;
        system("cls");
        printf("You come upon %s.\n", enemy[0].name);
        getch();
        do
        {
             system("cls");
             printf("Round %d:\n%s hp: %d    %s hp: %d\n", round, player.name, player.health, enemy[0].name, enemy[0].health);
             printf("\n1. ATTACK\n2. FLEE\n");
             gets(option);
             if ((strcmp(upper(option),"ATTACK")==0)||(strcmp(upper(option),"1")==0))
             {
                  system("cls");
                  roll1=diceroll("1d20")+player.strength;  //first turn
                  roll2=diceroll("1d20")+enemy[0].agility;
                  printf("You roll %d.\nEnemy roll %d.\n", roll1, roll2);
                  if (roll1>=roll2)  //player hits enemy
                  {
                        if(diceroll("1d100")>90)  //critical hit
                        {
                                damage=diceroll(player.equipment[WEAPON].damage);
                                printf("You hit your enemy hard, causing double damage! 2x%d\n", damage);
                                enemy[0].health=enemy[0].health-2*damage;
                        }
                        else
                        {
                                damage=diceroll(player.equipment[WEAPON].damage);
                                printf("You hit enemy by %d.\n", damage);
                                enemy[0].health=enemy[0].health-damage;
                        }
                  }
                  else printf("%s parries all your hits.\n", enemy[0].name);
                  
                  if (enemy[0].health<=0)  //player wins
                  {
                        enemy[0].health=0;
                        treasure=diceroll("1d5");
                        printf("You win!\nYou gain 10 exp. You earn %d gold.\n", treasure);
                        player.exp=player.exp+10;
                        player.gold=player.gold+treasure;
                        player=levelup(player);
                        result=1;
                        break;
                  }
                  
                  roll1=diceroll("1d20")+enemy[0].strength;  //second turn
                  roll2=diceroll("1d20")+player.agility+player.equipment[ARMOR].armor+player.equipment[SHIELD].armor;
                  printf("\nEnemy roll %d.\nYou roll %d.\n", roll1, roll2);
                  if (roll1>=roll2)  //enemy hits player
                  {
                        if(diceroll("1d100")>90)  //critical hit
                        {
                                damage=diceroll(enemy[0].damage);
                                printf("%s's furiously attacks, cause double damage! 2x%d\n", enemy[0].name, damage);
                                player.health=player.health-2*damage;
                        }
                        else
                        {
                                damage=diceroll(enemy[0].damage);
                                printf("%s hits you by %d.\n", enemy[0].name, damage);
                                player.health=player.health-damage;
                        }
                  }
                  else printf("You dodge %s's attack.\n", enemy[0].name);
                  if (player.health<=0)  //player looses
                  {
                        player.health=0;
                        printf("You loose!\n");
                        result=0;
                        break;
                  }
                  round++;
                  getch();
             }
             else if ((strcmp(upper(option),"FLEE")==0)||(strcmp(upper(option),"2")==0))
             {
                  printf("Are you sure, that you want to flee? You will not gain experiance from this battle.\n");
                  gets(cond);
                  if ((strcmp(upper(cond),"YES")==0)||(strcmp(upper(cond),"Y")==0)) break;
             }
             else if(strcmp(upper(option),"CARD")==0) card(player);
        } while ((result!=1)&&(result!=0));
    }
    else  //fighting with armies
    {
          round=1;
          provoke=0;
          for(i=0;i<4;i++) move[i]=song[i]=curse[i]=0;
          expgain=10*(enemy[0].number+enemy[1].number);
          system("cls");
          if(enemy[1].number>0) printf("You come upon %d %s and %d %s\n", enemy[0].number, enemy[0].name, enemy[1].number, enemy[1].name);
          else printf("You come upon %d %s\n", enemy[0].number, enemy[0].name);
          pikemen.maxhealth=pikemen.health=archers.maxhealth=archers.health=player.health;
          pikemen.number=player.army[PIKEMEN];
          archers.number=player.army[ARCHERS];
          sprintf(archersdamage, "1d%d", player.lvl);  //defining damage done by archers
          getch();
          do
          {
             system("cls");
             printf("Round: %d\n%d Pikemen: %d/%dhp     %d %s: %d/%dhp\n%d Archers: %d/%dhp     %d %s: %d/%dhp\nBard: %d, Shaman: %d\n", round, player.army[PIKEMEN], pikemen.health, pikemen.maxhealth, enemy[0].number, enemy[0].name, enemy[0].health, enemy[0].maxhealth, player.army[ARCHERS], archers.health, archers.maxhealth, enemy[1].number, enemy[1].name, enemy[1].health, enemy[1].maxhealth, player.army[BARD], player.army[SHAMAN]);
             printf("\n1. SELECT unit.\n2. END round.\n3. FLEE.\n");
             gets(option);
             if ((strcmp(upper(option),"SELECT")==0)||(strcmp(upper(option),"1")==0))
             {
                  while (TRUE)
                  {
                        system("cls");
                        if((player.army[PIKEMEN]!=0)&&(move[0]==0)) printf("1. PIKEMEN (%d)\n", player.army[PIKEMEN]);
                        if((player.army[ARCHERS]!=0)&&(move[1]==0)) printf("2. ARCHERS (%d)\n", player.army[ARCHERS]);
                        if((player.army[BARD]!=0)&&(move[2]==0)) printf("3. BARD\n");
                        if((player.army[SHAMAN]!=0)&&(move[3]==0)) printf("4. SHAMAN\n");
                        if((player.pact[DRAGON]==1)&&(drag==1)) printf("5. Summon DRAGON\n");
                        printf("0. BACK to battle menu.\n");
                        gets(select);
                        if ((strcmp(upper(select),"PIKEMEN")==0)||(strcmp(upper(select),"1")==0)) //Pikemen menu
                        {
                             if(player.army[PIKEMEN]!=0)
                             {
                                   if (move[0]==0) while(TRUE)
                                   {
                                        system("cls");
                                        if(enemy[1].number>0) //there are archers in enemy's army
                                        {
                                              printf("Selected unit: Pikemen\n\n1. ATTACK\n2. PROVOKE archers.\n3. BACK to select menu.\n");
                                              gets(plmove);
                                              if ((strcmp(upper(plmove),"ATTACK")==0)||(strcmp(upper(plmove),"1")==0))
                                              {
                                                   if(enemy[0].number>0)
                                                   {
                                                        if (song[0]>0) roll1=diceroll("1d20")+player.strength+player.pact[ELVEN]*5+2;  //pikemen attack roll
                                                        else roll1=diceroll("1d20")+player.strength+player.pact[ELVEN]*5;
                                                        if ((curse[1]>0)||(curse[3]>0)) roll2=diceroll("1d20")+enemy[0].agility+player.lvl-2;  //enemy defence roll
                                                        else roll2=diceroll("1d20")+enemy[0].agility+player.lvl;
                                                        if (roll1>=roll2)  //pikemen hit
                                                        {
                                                             if(diceroll("1d100")>90)  //critical hit
                                                             {
                                                                     damage=diceroll(player.equipment[WEAPON].damage)*player.army[PIKEMEN];
                                                                     printf("Pikemen hit %s hard, causing double damage! 2x%d\n", enemy[0].name, damage);
                                                                     i=enemy[0].number;
                                                                     enemy[0]=dmgtk(enemy[0],2*damage);
                                                                     printf("%d %s perish.\n", i-enemy[0].number, enemy[0].name);
                                                             }
                                                             else  
                                                             {
                                                                     damage=diceroll(player.equipment[WEAPON].damage)*player.army[PIKEMEN];
                                                                     printf("Pikemen hit %s by %d.\n", enemy[0].name, damage);
                                                                     i=enemy[0].number;
                                                                     enemy[0]=dmgtk(enemy[0],damage);
                                                                     printf("%d %s perish.\n", i-enemy[0].number, enemy[0].name);
                                                             }
                                                        }
                                                        else printf("%s parries all hits.\n", enemy[0].name);
                                                        move[0]=1;
                                                        getch();
                                                        break;
                                                   }
                                                   else  //there are no melee troops in enemy's army
                                                   {
                                                         if (song[0]>0) roll1=diceroll("1d20")+player.strength+player.pact[ELVEN]*5+2; //pikemen attack roll
                                                         else roll1=diceroll("1d20")+player.strength+player.pact[ELVEN]*5;
                                                         if (curse[3]>0) roll2=diceroll("1d20")+enemy[1].agility+player.lvl-2;  //enemy defence roll
                                                         else roll2=diceroll("1d20")+enemy[1].agility+player.lvl;
                                                         if (roll1>=roll2)  //pikemen hit
                                                         {
                                                             if(diceroll("1d100")>90)  //critical hit
                                                             {
                                                                     damage=diceroll(player.equipment[WEAPON].damage)*player.army[PIKEMEN];
                                                                     printf("Pikemen hit %s hard, causing double damage! 2x%d\n", enemy[1].name, damage);
                                                                     i=enemy[1].number;
                                                                     enemy[1]=dmgtk(enemy[1],2*damage);
                                                                     printf("%d %s perish.\n", i-enemy[1].number, enemy[1].name);
                                                             }
                                                             else  
                                                             {
                                                                     damage=diceroll(player.equipment[WEAPON].damage)*player.army[PIKEMEN];
                                                                     printf("Pikemen hit %s by %d.\n", enemy[1].name, damage);
                                                                     i=enemy[1].number;
                                                                     enemy[1]=dmgtk(enemy[1],damage);
                                                                     printf("%d %s perish.\n", i-enemy[1].number, enemy[1].name);
                                                             }
                                                         }
                                                         else printf("%s parries all hits.\n", enemy[1].name);
                                                         move[0]=1;
                                                         getch();
                                                         break;
                                                   }
                                              }
                                              else if ((strcmp(upper(plmove),"PROVOKE")==0)||(strcmp(upper(plmove),"2")==0))
                                              {
                                                   provoke=1;
                                                   move[0]=1;
                                                   getch();
                                                   break;
                                              }
                                              else if (strcmp(upper(plmove),"CARD")==0) card(player);
                                              else if ((strcmp(upper(plmove),"BACK")==0)||(strcmp(upper(plmove),"3")==0)) break;
                                        }
                                        else //there are no archers in enemy's army
                                        {
                                             printf("Selected unit: Pikemen\n\n1. ATTACK\n2. BACK to select menu.\n");
                                             gets(plmove);
                                             if ((strcmp(upper(plmove),"ATTACK")==0)||(strcmp(upper(plmove),"1")==0))
                                             {
                                                   if (song[0]>0) roll1=diceroll("1d20")+player.strength+player.pact[ELVEN]*5+2;  //pikemen attack roll
                                                   else roll1=diceroll("1d20")+player.strength+player.pact[ELVEN]*5;
                                                   if ((curse[1]>0)||(curse[3]>0)) roll2=diceroll("1d20")+enemy[0].agility+player.lvl-2;  //enemy defence roll
                                                   else roll2=diceroll("1d20")+enemy[0].agility+player.lvl;
                                                   if (roll1>=roll2)  //pikemen hit
                                                   {
                                                         if(diceroll("1d100")>90)  //critical hit
                                                         {
                                                              damage=diceroll(player.equipment[WEAPON].damage)*player.army[PIKEMEN];
                                                              printf("Pikemen hit %s hard, causing double damage! 2x%d\n", enemy[0].name, damage);
                                                              i=enemy[0].number;
                                                              enemy[0]=dmgtk(enemy[0],2*damage);
                                                              printf("%d %s perish.\n", i-enemy[0].number, enemy[0].name);
                                                         }
                                                         else  
                                                         {
                                                              damage=diceroll(player.equipment[WEAPON].damage)*player.army[PIKEMEN];
                                                              printf("Pikemen hit %s by %d.\n", enemy[0].name, damage);
                                                              i=enemy[0].number;
                                                              enemy[0]=dmgtk(enemy[0],damage);
                                                              printf("%d %s perish.\n", i-enemy[0].number, enemy[0].name);
                                                         }
                                                   }
                                                   else printf("%s parries all hits.\n", enemy[0].name);
                                                   move[0]=1;
                                                   getch();
                                                   break;
                                             }
                                             else if (strcmp(upper(plmove),"CARD")==0) card(player);
                                             else if ((strcmp(upper(plmove),"BACK")==0)||(strcmp(upper(plmove),"3")==0)) break;
                                        }
                                   }
                                   else {printf("Your Pikemen had just moved in this turn.\n"); getch();}
                             }
                             else {printf("You do not have Pikemen in your army.\n"); getch();}
                             break;
                        }
                        else if ((strcmp(upper(select),"ARCHERS")==0)||(strcmp(upper(select),"2")==0)) //Archers menu
                        {
                             if(player.army[ARCHERS]!=0)
                             {
                                   if(move[1]==0) while(TRUE)
                                   {
                                         system("cls");
                                         printf("Selected unit: Archers\n1. ATTACK\n2. BACK to select menu.\n");
                                         gets(plmove);
                                         if ((strcmp(upper(plmove),"ATTACK")==0)||(strcmp(upper(plmove),"1")==0))
                                         {
                                               if(enemy[0].number>0) printf("1. Attack FIRST line (%s)\n", enemy[0].name);
                                               if(enemy[1].number>0) printf("2. Attack SECOND line (%s)\n", enemy[1].name);
                                               gets(choice);
                                               if ((strcmp(upper(choice),"FIRST")==0)||(strcmp(upper(choice),"1")==0))
                                               {
                                                   if(enemy[0].number>0)
                                                   {
                                                         if (song[2]>0) roll1=diceroll("1d20")+player.strength+player.pact[ELVEN]*5+2;  //Archers attack roll
                                                         else roll1=diceroll("1d20")+player.strength+player.pact[ELVEN]*5;
                                                         if ((curse[1]>0)||(curse[3]>0)) roll2=diceroll("1d20")+enemy[0].agility+player.lvl-2; //enemy defence roll
                                                         else roll2=diceroll("1d20")+enemy[0].agility+player.lvl;
                                                         if (roll1>=roll2)  //archers hit
                                                         {
                                                              if(diceroll("1d100")>90)  //critical hit
                                                              {
                                                                    damage=diceroll(archersdamage)*player.army[ARCHERS];
                                                                    printf("Pikemen hit %s hard, causing double damage! 2x%d\n", enemy[0].name, damage);
                                                                    i=enemy[0].number;
                                                                    enemy[0]=dmgtk(enemy[0],2*damage);
                                                                    printf("%d %s perish.\n", i-enemy[0].number, enemy[0].name);
                                                              }
                                                              else  
                                                              {
                                                                    damage=diceroll(archersdamage)*player.army[ARCHERS];
                                                                    printf("Pikemen hit %s by %d.\n", enemy[0].name, damage);
                                                                    i=enemy[0].number;
                                                                    enemy[0]=dmgtk(enemy[0],damage);
                                                                    printf("%d %s perish.\n", i-enemy[0].number, enemy[0].name);
                                                              }
                                                         }
                                                         else printf("%s defends.\n", enemy[0].name);
                                                         move[1]=1;
                                                         getch();
                                                         break;
                                                   }
                                                   else printf("There are no troops in enemy's first line army.\n");
                                               }
                                               else if ((strcmp(upper(choice),"SECOND")==0)||(strcmp(upper(choice),"2")==0))
                                               {
                                                    if(enemy[1].number>0)
                                                    {
                                                          if (song[2]>0) roll1=diceroll("1d20")+player.strength+player.pact[ELVEN]*5+2;  //Archers attack roll
                                                          else roll1=diceroll("1d20")+player.strength+player.pact[ELVEN]*5;
                                                          if (curse[3]>0) roll2=diceroll("1d20")+enemy[1].agility+player.lvl-2;  //enemy defence roll
                                                          else roll2=diceroll("1d20")+enemy[1].agility+player.lvl;
                                                          if (roll1>=roll2)  //archers hit
                                                          {
                                                                if(diceroll("1d100")>90)  //critical hit
                                                                {
                                                                      damage=diceroll(archersdamage)*player.army[ARCHERS];
                                                                      printf("Pikemen hit %s hard, causing double damage! 2x%d\n", enemy[1].name, damage);
                                                                      i=enemy[1].number;
                                                                      enemy[1]=dmgtk(enemy[1],2*damage);
                                                                      printf("%d %s perish.\n", i-enemy[1].number, enemy[1].name);
                                                                }
                                                                else  
                                                                {
                                                                      damage=diceroll(archersdamage)*player.army[ARCHERS];
                                                                      printf("Pikemen hit %s by %d.\n", enemy[1].name, damage);
                                                                      i=enemy[1].number;
                                                                      enemy[1]=dmgtk(enemy[1],damage);
                                                                      printf("%d %s perish.\n", i-enemy[1].number, enemy[1].name);
                                                                }
                                                          }
                                                          else printf("%s defends.\n", enemy[1].name);
                                                          move[1]=1;
                                                          getch();
                                                          break;
                                                    }
                                                    else printf("There are no troops in enemy's second line army.\n");
                                               }
                                               else printf("Not a proper option.\n");
                                         }
                                         else if (strcmp(upper(plmove),"CARD")==0) card(player);
                                         else if ((strcmp(upper(plmove),"BACK")==0)||(strcmp(upper(plmove),"2")==0)) break;
                                   }
                                   else {printf("Your Archers had just moved in this turn.\n"); getch();}
                             }
                             else {printf("You do not have Archers in your army.\n"); getch();}
                             break;
                        }
                        else if ((strcmp(upper(select),"BARD")==0)||(strcmp(upper(select),"3")==0)) //Bard menu
                        {
                             if(player.army[BARD]!=0)
                             {
                                  if(move[2]==0) while(TRUE)
                                  {
                                         system("cls");
                                         printf("Selected unit: Bard\n");
                                         if (player.lvl<7)
                                         {
                                               printf("1. Song of COURAGE (strenghtens hit chance of your Pikemen)\n2. Song of HOPE (gives defence bonus to your Pikemen)\n3. BACK to select menu.\n");
                                               gets(plmove);
                                               if ((strcmp(upper(plmove),"COURAGE")==0)||(strcmp(upper(plmove),"1")==0))
                                               {
                                                    song[0]=1;
                                                    move[2]=1;
                                               }
                                               else if ((strcmp(upper(plmove),"HOPE")==0)||(strcmp(upper(plmove),"2")==0))
                                               {
                                                    song[1]=1;
                                                    move[2]=1;
                                               }
                                               else if (strcmp(upper(plmove),"CARD")==0) card(player);
                                               else if ((strcmp(upper(plmove),"BACK")==0)||(strcmp(upper(plmove),"3")==0)) break;
                                         }
                                         else if((player.lvl==7)||(player.lvl==8))
                                         {
                                               printf("1. Song of COURAGE (+ to hit chance of Pikemen)\n2. Song of HOPE (+ to defence of Pikemen)\n3. Song of ACCURACY (+ to hit chance of Archers)\n4. Song of PROTECTION (+ to defence of Archers)\n5. BACK to select menu.\n");
                                               gets(plmove);
                                               if ((strcmp(upper(plmove),"COURAGE")==0)||(strcmp(upper(plmove),"1")==0))
                                               {
                                                     song[0]=1;
                                                     move[2]=1;
                                               }
                                               else if ((strcmp(upper(plmove),"HOPE")==0)||(strcmp(upper(plmove),"2")==0))
                                               {
                                                    song[1]=1;
                                                    move[2]=1;
                                               }
                                               else if ((strcmp(upper(plmove),"ACCURACY")==0)||(strcmp(upper(plmove),"3")==0))
                                               {
                                                    song[2]=1;
                                                    move[2]=1;
                                               }
                                               else if ((strcmp(upper(plmove),"PROTECTION")==0)||(strcmp(upper(plmove),"4")==0))
                                               {
                                                    song[3]=1;
                                                    move[2]=1;
                                               }
                                               else if (strcmp(upper(plmove),"CARD")==0) card(player);
                                               else if ((strcmp(upper(plmove),"BACK")==0)||(strcmp(upper(plmove),"5")==0)) break;
                                         }
                                         else
                                         {
                                               printf("1. Song of COURAGE (+ to hit chance of Pikemen)\n2. Song of HOPE (+ to defence of Pikemen)\n3. Song of ACCURACY (+ to hit chance of Archers)\n4. Song of PROTECTION (+ to defence of Archers)\n5. BACK to select menu.\nEach of songs lasts for 2 turns.\n");
                                               gets(plmove);
                                               if ((strcmp(upper(plmove),"COURAGE")==0)||(strcmp(upper(plmove),"1")==0))
                                               {
                                                     song[0]=2;
                                                     move[2]=1;
                                               }
                                               else if ((strcmp(upper(plmove),"HOPE")==0)||(strcmp(upper(plmove),"2")==0))
                                               {
                                                    song[1]=2;
                                                    move[2]=1;
                                               }
                                               else if ((strcmp(upper(plmove),"ACCURACY")==0)||(strcmp(upper(plmove),"3")==0))
                                               {
                                                    song[2]=2;
                                                    move[2]=1;
                                               }
                                               else if ((strcmp(upper(plmove),"PROTECTION")==0)||(strcmp(upper(plmove),"4")==0))
                                               {
                                                    song[3]=2;
                                                    move[2]=1;
                                               }
                                               else if (strcmp(upper(plmove),"CARD")==0) card(player);
                                               else if ((strcmp(upper(plmove),"BACK")==0)||(strcmp(upper(plmove),"5")==0)) break;
                                         } break;
                                  }
                                  else {printf("Your Bard had just moved in this turn.\n"); getch();}
                             }
                             else {printf("You do not have Bard in your army.\n"); getch();}
                             break;
                        }
                        else if ((strcmp(upper(select),"SHAMAN")==0)||(strcmp(upper(select),"4")==0)) //Shaman menu
                        {
                             if(player.army[SHAMAN]!=0)
                             {
                                  if(move[3]==0) while(TRUE)
                                  {
                                         system("cls");
                                         printf("Selected unit: Shaman\n");
                                         if (player.lvl<7)
                                         {
                                               printf("1. Curse of WEAKNESS (- to hit chance of second line of enemy)\n2. Curse of FEAR (- to defence of first line of enemy)\n3. BACK to select menu.\n");
                                               gets(plmove);
                                               if ((strcmp(upper(plmove),"WEAKNESS")==0)||(strcmp(upper(plmove),"1")==0))
                                               {
                                                    curse[0]=1;
                                                    move[3]=1;
                                               }
                                               else if ((strcmp(upper(plmove),"FEAR")==0)||(strcmp(upper(plmove),"2")==0))
                                               {
                                                    curse[1]=1;
                                                    move[3]=1;
                                               }
                                               else if (strcmp(upper(plmove),"CARD")==0) card(player);
                                               else if ((strcmp(upper(plmove),"BACK")==0)||(strcmp(upper(plmove),"3")==0)) break;
                                         }
                                         else if ((player.lvl>=7)&&(player.lvl<=9))
                                         {
                                               printf("1. Curse of WEAKNESS (- to hit chance of second line of enemy)\n2. Curse of FEAR (- to defence of first line of enemy)\n3. Curse of DISORIENT (10%% that enemy unit may not attack)\n4. BACK to select menu.\n");
                                               gets(plmove);
                                               if ((strcmp(upper(plmove),"WEAKNESS")==0)||(strcmp(upper(plmove),"1")==0))
                                               {
                                                    curse[0]=1;
                                                    move[3]=1;
                                               }
                                               else if ((strcmp(upper(plmove),"FEAR")==0)||(strcmp(upper(plmove),"2")==0))
                                               {
                                                    curse[1]=1;
                                                    move[3]=1;
                                               }
                                               else if ((strcmp(upper(plmove),"DISORIENT")==0)||(strcmp(upper(plmove),"3")==0))
                                               {
                                                    if(diceroll("1d100")<=10)
                                                    {
                                                          curse[2]=1;
                                                    }
                                                    else printf("The Curse of Disorient failed.\n");
                                                    move[3]=1;
                                               }
                                               else if (strcmp(upper(plmove),"CARD")==0) card(player);
                                               else if ((strcmp(upper(plmove),"BACK")==0)||(strcmp(upper(plmove),"4")==0)) break;
                                         }
                                         else
                                         {
                                               printf("1. Curse of WEAKNESS (- to hit chance of second line of enemy)\n2. Curse of FEAR (- to defence of first line of enemy)\n3. Curse of DISORIENT (20%% that enemy unit may not attack)\n4. Curse of DOOM (10%% that all enemy units have problems with attack and defence, lasts for 2 turns)\n5. BACK to select menu.\n");
                                               gets(plmove);
                                               if ((strcmp(upper(plmove),"WEAKNESS")==0)||(strcmp(upper(plmove),"1")==0))
                                               {
                                                     curse[0]=1;
                                                     move[3]=1;
                                               }
                                               else if ((strcmp(upper(plmove),"FEAR")==0)||(strcmp(upper(plmove),"2")==0))
                                               {
                                                    curse[1]=1;
                                                    move[3]=1;
                                               }
                                               else if ((strcmp(upper(plmove),"DISORIENT")==0)||(strcmp(upper(plmove),"3")==0))
                                               {
                                                    if(diceroll("1d100")<=20)
                                                    {
                                                          curse[2]=1;
                                                    }
                                                    else {printf("The Curse of Disorient failed.\n"); getch();}
                                                    move[3]=1;
                                               }
                                               else if ((strcmp(upper(plmove),"DOOM")==0)||(strcmp(upper(plmove),"4")==0))
                                               {
                                                    if(diceroll("1d100")<=10)
                                                    {
                                                          curse[3]=2;
                                                    }
                                                    else {printf("The Curse of Doom failed.\n"); getch();}
                                                    move[3]=1;
                                               }
                                               else if (strcmp(upper(plmove),"CARD")==0) card(player);
                                               else if ((strcmp(upper(plmove),"BACK")==0)||(strcmp(upper(plmove),"5")==0)) break;
                                               move[0]=0;
                                         } break;
                                  }
                                  else {printf("Your Shaman had just moved in this turn.\n"); getch();}
                             }
                             else {printf("You do not have Shaman in your army.\n"); getch();}
                             break;
                        }
                        else if ((strcmp(upper(select),"DRAGON")==0)||(strcmp(upper(select),"5")==0)) //Summoning Dragon
                        {
                             if(player.pact[DRAGON]==1)
                             {
                                  if(drag==1)
                                  {
                                        i=enemy[0].number;
                                        r=enemy[1].number;
                                        if (enemy[0].number>0) {enemy[0].number=enemy[0].number-rand()%enemy[0].number; printf("%d %s run in fear.\n", i-enemy[0].number, enemy[0].name);}
                                        if (enemy[1].number>0) {enemy[1].number=enemy[1].number-rand()%enemy[1].number; printf("%d %s run in fear.\n", r-enemy[1].number, enemy[0].name);}
                                        drag=0;
                                        getch();
                                        break;
                                  }
                                  else {printf("You can call a Dragon only once per battle.\n"); getch();}
                             }
                             else {printf("You do not have Dragon pact yet.\n"); getch();}
                        }
                        else if (strcmp(upper(select),"CARD")==0) card(player);
                        else if ((strcmp(upper(select),"BACK")==0)||(strcmp(upper(select),"0")==0)) break;
                  }
             }
             else if ((strcmp(upper(option),"END")==0)||(strcmp(upper(option),"2")==0))  //enemy's move
             {
                  system("cls");
                  if(enemy[0].number>0) //if there is first line enemy
                  {
                       if(player.army[PIKEMEN]>0) //Enemy's first line attack Pikemen
                       {
                            if (curse[3]>0) roll1=diceroll("1d20")+enemy[0].strength-2;  //enemy attack roll
                            else roll1=diceroll("1d20")+enemy[0].strength;
                            if (song[1]>0) roll2=diceroll("1d20")+player.agility+2;  //Pikemen defence roll
                            else roll2=diceroll("1d20")+player.agility;
                            if (roll1>=roll2)  //enemy hit
                            {
                               if(diceroll("1d100")>90)  //critical hit
                               {
                                    damage=diceroll(enemy[0].damage)*enemy[0].number;
                                    printf("%s hit your Pikemen hard, causing double damage! 2x%d\n", enemy[0].name, damage);
                                    i=pikemen.number;
                                    pikemen=dmgtk(pikemen,2*damage); //damage to Pikemen
                                    printf("%d Pikemen perish.\n", i-pikemen.number);
                                    player.army[PIKEMEN]=pikemen.number;
                               }
                               else  
                               {
                                    damage=diceroll(archersdamage)*player.army[ARCHERS];
                                    printf("%s hit your Pikemen by %d.\n", enemy[0].name, damage);
                                    i=pikemen.number;
                                    pikemen=dmgtk(pikemen,damage); //damage to Pikemen
                                    printf("%d Pikemen perish.\n", i-pikemen.number);
                                    player.army[PIKEMEN]=pikemen.number;
                               }
                            }
                            else printf("Your Pikemen defend themselves.\n");
                       }
                       else //if there are no Pikemen, enemy attack Archers
                       {
                            if (curse[3]>0) roll1=diceroll("1d20")+enemy[0].strength-2;  //enemy attack roll
                            else roll1=diceroll("1d20")+enemy[0].strength;
                            if (song[3]>0) roll2=diceroll("1d20")+player.agility+2;  //Archers defence roll
                            else roll2=diceroll("1d20")+player.agility;
                            if (roll1>=roll2)  //enemy hit
                            {
                               if(diceroll("1d100")>90)  //critical hit
                               {
                                    damage=diceroll(enemy[0].damage)*enemy[0].number;
                                    printf("%s hit your Archers hard, causing double damage! 2x%d\n", enemy[0].name, damage);
                                    i=archers.number;
                                    archers=dmgtk(archers,2*damage); //damage to Archers
                                    printf("%d Archers perish.\n", i-archers.number);
                                    player.army[ARCHERS]=archers.number;
                               }
                               else  
                               {
                                    damage=diceroll(enemy[0].damage)*enemy[0].number;
                                    printf("%s hit your Archers by %d.\n", enemy[0].name, damage);
                                    i=archers.number;
                                    archers=dmgtk(archers,damage); //damage to Archers
                                    printf("%d Archers perish.\n", i-archers.number);
                                    player.army[ARCHERS]=archers.number;
                               }
                            }
                            else printf("Your Archers defend themselves.\n");
                       }
                  }
                  if(enemy[1].number>0) //if there is enemy's second line
                  {
                        if(provoke==1) //enemy second line is provoked by Pikemen
                        {
                            if (curse[3]>0) roll1=diceroll("1d20")+enemy[1].strength-2;  //enemy attack roll
                            else roll1=diceroll("1d20")+enemy[1].strength;
                            if (song[1]>0) roll2=diceroll("1d20")+player.agility+2;  //Pikemen defence roll
                            else roll2=diceroll("1d20")+player.agility;
                            if (roll1>=roll2)  //enemy hit
                            {
                                if(diceroll("1d100")>90)  //critical hit
                                {
                                      damage=diceroll(enemy[1].damage)*enemy[1].number;
                                      printf("%s hit your Pikemen hard, causing double damage! 2x%d\n", enemy[1].name, damage);
                                      i=pikemen.number;
                                      pikemen=dmgtk(pikemen,2*damage); //damage to Pikemen
                                      printf("%d Pikemen perish.\n", i-pikemen.number);
                                      player.army[PIKEMEN]=pikemen.number;
                                }
                                else  
                                {
                                      damage=diceroll(enemy[1].damage)*enemy[1].number;
                                      printf("%s hit your Pikemen by %d.\n", enemy[1].name, damage);
                                      i=pikemen.number;
                                      pikemen=dmgtk(pikemen,damage); //damage to Pikemen
                                      printf("%d Pikemen perish.\n", i-pikemen.number);
                                      player.army[PIKEMEN]=pikemen.number;
                                }
                            }
                            else printf("Your Pikemen defend themselves.\n");
                        }
                        else //if not provoked
                        {
                            r=diceroll("1d100");
                            if(r<60) //enemy attack Archers
                            {
                                  if (curse[3]>0) roll1=diceroll("1d20")+enemy[1].strength-2;  //enemy attack roll
                                  else roll1=diceroll("1d20")+enemy[1].strength;
                                  if (song[3]>0) roll2=diceroll("1d20")+player.agility+2;  //Archers defence roll
                                  else roll2=diceroll("1d20")+player.agility;
                                  if (roll1>=roll2)  //enemy hit
                                  {
                                        if(diceroll("1d100")>90)  //critical hit
                                        {
                                             damage=diceroll(enemy[1].damage)*enemy[1].number;
                                             printf("%s hit your Archers hard, causing double damage! 2x%d\n", enemy[1].name, damage);
                                             i=archers.number;
                                             archers=dmgtk(archers,2*damage); //damage to Archers
                                             printf("%d Archers perish.\n", i-archers.number);
                                             player.army[ARCHERS]=archers.number;
                                        }
                                        else  
                                        {
                                             damage=diceroll(enemy[1].damage)*enemy[1].number;
                                             printf("%s hit your Archers by %d.\n", enemy[1].name, damage);
                                             i=archers.number;
                                             archers=dmgtk(archers,damage); //damage to Archers
                                             printf("%d Archers perish.\n", i-archers.number);
                                             player.army[ARCHERS]=archers.number;
                                        }
                                  }
                                  else printf("Your Archers defend themselves.\n");
                            }
                            else if(r<90) //enemy attack Pikemen
                            {
                                  if (curse[3]>0) roll1=diceroll("1d20")+enemy[1].strength-2;  //enemy attack roll
                                  else roll1=diceroll("1d20")+enemy[1].strength;
                                  if (song[1]>0) roll2=diceroll("1d20")+player.agility+2;  //Pikemen defence roll
                                  else roll2=diceroll("1d20")+player.agility;
                                  if (roll1>=roll2)  //enemy hit
                                  {
                                         if(diceroll("1d100")>90)  //critical hit
                                         {
                                            damage=diceroll(enemy[1].damage)*enemy[1].number;
                                            printf("%s hit your Pikemen hard, causing double damage! 2x%d\n", enemy[1].name, damage);
                                            i=pikemen.number;
                                            pikemen=dmgtk(pikemen,2*damage); //damage to Pikemen
                                            printf("%d Pikemen perish.\n", i-pikemen.number);
                                            player.army[PIKEMEN]=pikemen.number;
                                         }
                                         else  
                                         {
                                            damage=diceroll(enemy[1].damage)*enemy[1].number;
                                            printf("%s hit your Pikemen by %d.\n", enemy[1].name, damage);
                                            i=pikemen.number;
                                            pikemen=dmgtk(pikemen,damage); //damage to Pikemen
                                            printf("%d Pikemen perish.\n", i-pikemen.number);
                                            player.army[PIKEMEN]=pikemen.number;
                                         }
                                  }
                                  else printf("Your Pikemen defend themselves.\n");
                            }
                            else if(r<95) //enemy kill Bard
                            {
                                  if (player.army[BARD]==0) printf("%s miss their target.\n", enemy[1].name);
                                  else
                                  {
                                          printf("%s shoot down your Bard!\n", enemy[1].name);
                                          player.army[BARD]=0;
                                  }
                            }
                            else //enemy kill Shaman
                            {
                                  if (player.army[SHAMAN]==0) printf("%s miss their target.\n", enemy[1].name);
                                  else
                                  {
                                           printf("%s shoot down your Shaman!\n", enemy[1].name);
                                           player.army[SHAMAN]=0;
                                  }
                            }
                        }
                  }
                  else if((enemy[0].number<1)&&(enemy[1].number<1)) //player wins
                  {
                        treasure=10*diceroll("1d10")*(rand()%player.lvl+1);
                        printf("Congratulations! You have won a battle!\nYou gain %d exp and %d gold.\n", expgain, treasure);
                        player.exp=player.exp+expgain;
                        player.gold=player.gold+treasure;
                        player.charisma=10*player.lvl-player.army[PIKEMEN]-2*player.army[ARCHERS]-5*player.army[BARD]-5*player.army[SHAMAN]+100*player.pact[ALLIANCE_ARMY];
                        save(player);
                        result=1;
                        break;
                  }
                  printf("Round %d ends.\n", round);
                  for(i=0;i<4;i++)
                  {
                       if(song[i]>0) song[i]--;
                       if(i!=3) if(curse[i]>0) curse[i]=0;
                       move[i]=0;
                  }
                  provoke=0;
                  round++;
                  getch();
             }
             else if ((strcmp(upper(option),"FLEE")==0)||(strcmp(upper(option),"3")==0))
             {
                  printf("Are you sure, that you want to flee? You may loose units left behind and you will not gain experiance from this battle.\n");
                  gets(cond);
                  if ((strcmp(upper(cond),"YES")==0)||(strcmp(upper(cond),"Y")==0))
                  {
                      i=player.army[PIKEMEN];
                      r=player.army[ARCHERS];
                      i=i-rand()%player.army[PIKEMEN];
                      r=r-rand()%player.army[ARCHERS];
                      printf("%d Pikemen and %d Archers lost.\n", player.army[PIKEMEN]-i, player.army[ARCHERS]-r);
                      player.army[PIKEMEN]=i;
                      player.army[ARCHERS]=r;
                      break;
                  }
             }
             else if (strcmp(upper(option),"CARD")==0) card(player);
             else if (strcmp(upper(option),"MASTEROFPOWER")==0)
             {
                  treasure=10*diceroll("1d10")*(rand()%player.lvl+1);
                  printf("Cheater! You have won a battle!\nYou gain %d exp and %d gold.\n", expgain, treasure);
                  player.exp=player.exp+expgain;
                  player.gold=player.gold+treasure;
                  player.charisma=10*player.lvl-player.army[PIKEMEN]-2*player.army[ARCHERS]-5*player.army[BARD]-5*player.army[SHAMAN]+100*player.pact[ALLIANCE_ARMY];
                  save(player);
                  result=1;
                  break;
             }
          } while((result!=0)&&(result!=1));
    }
    player=levelup(player);
    save(player);
    return result;
}

bool check(struct acc tempAcc)
{
     struct acc checks;
     FILE *f;
     f = fopen("accounts", "rb");
     if(f==NULL) perror("Error opening file accounts");
     else
     {
         while(!feof(f))
         {
                fread(&checks,sizeof(struct acc),1,f);
                if(strcmp(tempAcc.login,checks.login)==0)
                      if(strcmp(tempAcc.pass,checks.pass)==0) {return TRUE; break;}
         }
         if (feof(f)) return FALSE;
     }
     fclose(f);
}

void create(struct acc tempAcc)
{
     FILE *f;
     char string[25];
     struct character temp;
     f = fopen("accounts", "rb");
     if (f==NULL)
     {
             fclose(f);
             f = fopen("accounts", "ab+");
             fwrite(&tempAcc,sizeof(struct acc),1,f);
             fclose(f);
             sprintf(string, "characters\\%s.txt", tempAcc.login);
             f = fopen(string, "wt");
             sscanf(tempAcc.login, "%s", &temp.name);
             temp.exp=-1;
             fwrite(&temp, 1, sizeof(struct character), f);
             fclose(f);
     }
     else if(!check(tempAcc))
     {
              fclose(f);
              f = fopen("accounts", "ab+");
              fwrite(&tempAcc,sizeof(struct acc),1,f);
              sprintf(string, "characters\\%s.txt", tempAcc.login);
              fclose(f);
              f = fopen(string, "wt");
              sscanf(tempAcc.login, "%s", &temp.name);
              temp.exp=-1;
              fwrite(&temp, 1, sizeof(struct character), f);
              fclose(f);
     }
     else {printf("Actually there exist account with that login.\n"); getch();}
     return;
}

struct character load(char name[])
{
       FILE *f;
       char string[25];
       struct character player;
       sprintf(string, "characters\\%s.txt", name);
       f=fopen(string, "r");
       fread(&player, 1, sizeof(struct character), f);
       fclose(f);
       return player;
}

struct character save(struct character player)
{
       FILE *f;
       char string[25];
       sprintf(string, "characters\\%s.txt", player.name);
       f=fopen(string, "wt");
       fwrite(&player, 1, sizeof(struct character), f);
       fclose(f);
}

struct character levelup(struct character player)
{
     while(level(player.exp)>player.lvl)
     {
         player.lvl=player.lvl+1;
         player.health=90+10*player.lvl;
         player.charisma=player.charisma+10;
         printf("\nCongratulations! You have advanced to level %d!\n", player.lvl);
         while(TRUE)
         {
             printf("Which of your attributes would you like to develop?\n1. STRENGTH\n2. AGILITY\n3. DIPLOMACY\n", player.lvl);
             gets(option);
             if ((strcmp(upper(option),"STRENGTH")==0)||(strcmp(upper(option),"1")==0))
             {
                player.strength=player.strength+1;
                printf("Your strength have grown to %d.\n", player.strength);
                save(player);
                break;
             }
             else if ((strcmp(upper(option),"AGILITY")==0)||(strcmp(upper(option),"2")==0))
             {
                player.agility=player.agility+1;
                printf("Your strength have grown to %d.\n", player.agility);
                save(player);
                break;
             }
             else if ((strcmp(upper(option),"DIPLOMACY")==0)||(strcmp(upper(option),"3")==0))
             {
                  if(player.diplomacy<4)
                  {
                       player.diplomacy=player.diplomacy+1;
                       printf("Your diplomacy have grown to %d.\n", player.diplomacy);
                       save(player);
                       break;
                  }
                  else printf("You cannot have more than 4 diplomacy points.\n");
             }
             else if (strcmp(upper(option),"CARD")==0) card(player);
             else ("You must choose one of three attributes.\n");
         }
     }
     return player;
}

struct unit dmgtk(struct unit unit, int damage)
{
       while(damage>0)
       {
             if(damage>unit.health)
             {
                    damage=damage-unit.health;
                    unit.number--;
                    unit.health=unit.maxhealth;
             }
             else
             {
                    unit.health=unit.health-damage;
                    damage=0;
             }
       }
       if (unit.number<0) unit.number=0;
       return unit;
}

void card(struct character player)
{
     int level, maxexp=0;
     system("cls");
     printf("Name: %s   Health: %d   Place: %s\n", player.name, player.health, player.place);
     for(level=1;level<=player.lvl; level++) maxexp=maxexp+level*100; //calculating exp needed to lvl up
     if (player.lvl>=10) printf("Level: %d   Experience: %d   You have reached the highest lvl.\n", player.lvl, player.exp);
     else printf("Level: %d   Experience: %d   Exp needed to next lvl: %d\n", player.lvl, player.exp,maxexp-player.exp);
     printf("Strength: %d   Agility: %d   Diplomacy: %d  Charisma: %d\n", player.strength, player.agility, player.diplomacy, player.charisma);
     printf("\nEquipment:\n");
     if (strcmp(player.equipment[WEAPON].name,"0")==0) printf("Weapon: Broken_Bone   ");
     else printf("Weapon: %s %s   ", player.equipment[WEAPON].name, player.equipment[WEAPON].damage);
     if (strcmp(player.equipment[ARMOR].name,"0")==0) printf("Armor: Cloth   ");
     else printf("Armor: %s %d   ", player.equipment[ARMOR].name, player.equipment[ARMOR].armor);
     if (strcmp(player.equipment[SHIELD].name,"0")==0) printf("Shield: None\n");
     else printf("Shield: %s %d\n", player.equipment[SHIELD].name, player.equipment[SHIELD].armor);
     printf("Gold: %d\n", player.gold);
     printf("\nArmy:\n%d pikemen, %d archers, %d bard, %d shaman\n", player.army[PIKEMEN], player.army[ARCHERS], player.army[BARD], player.army[SHAMAN]);
     printf("\nPacts:\nDwarven: %d Elven: %d Dragon: %d Alliance_army: %d\n", player.pact[DWARVEN], player.pact[ELVEN], player.pact[DRAGON], player.pact[ALLIANCE_ARMY]);
     getch();
     return;
}

void help (void)
{
     while(TRUE)
     {
         system("cls");
         printf("To play a game choose options listed by option number or typing the word in caps.\n");
         printf("Help menu:\n1. ATTRIBUTES.\n2. UNITS.\n3. PACTS\n4. How to WIN?\n5. EXIT.\n");
         gets(option);
         if ((strcmp(upper(option),"ATTRIBUTES")==0)||(strcmp(upper(option),"1")==0)) {printf("Strength increases hit chance.\nAgility verifies your dodge ability.\nDiplomacy is needed to obtain pacts.\nCharisma shows how many units you can recruit to your army.\n"); getch();}
         else if ((strcmp(upper(option),"UNITS")==0)||(strcmp(upper(option),"2")==0)) {printf("Pikemen can attack enemies that are next to them, or provoke distance shooters to attack them instead of other units in your army.\nArchers can attack every enemy's unit.\nLet Bard sing to support your units.\nShaman casts curses that spread among enmy's units.\n"); getch();}
         else if ((strcmp(upper(option),"PACTS")==0)||(strcmp(upper(option),"3")==0)) {printf("Pact with Dwarven priests enchants your armor.\nPact with Elven blacksmiths enchants your weapon.\nPact with Dragon enables summoning the Dragon to fear enemy units.\nPact with Allied army allows you to recruit Bard, Shaman and rises up your Charisma.\n"); getch();}
         else if ((strcmp(upper(option),"WIN")==0)||(strcmp(upper(option),"4")==0)) {printf("You must defeat forces which are defending the castle of Limestone Ridge.\n"); getch();}
         else if ((strcmp(upper(option),"EXIT")==0)||(strcmp(upper(option),"5")==0)) break;
     }
     return;
}

int diceroll (char dice[])
{
     int i, roll, edge, result;
     sscanf(dice, "%dd%d", &roll, &edge);
     result=0;
     for(i=0; i<roll; i++) result=result+(rand()%edge+1);
     return result;
}

int level(int exp)
{
    int level, maxexp=0;
    for(level=1;level<=10; level++)
    {
         maxexp=maxexp+level*100;
         if (exp<maxexp) break;
    }
    if(level>10) level=10;
    return level;
}

char * redname (char str[])
{
     int i;
     for(i=0; str[i]; i++)
     if (str[i]=='_') str[i]=' ';
     return str;
}

char * upper (char str[])
{
     int i;
     for( i=0; str[i]; i++)
     str[i] = toupper(str[i]);
     return str;
}
