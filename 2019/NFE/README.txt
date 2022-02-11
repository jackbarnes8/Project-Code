 ███▄    █  █    ██  ██▓     ██▓         █████▒██▓  ▄████  ██░ ██ ▄▄▄█████▓▓█████  ██▀███     ▓█████ ▒██   ██▒ ▄████▄  ▓█████  ██▓███  ▄▄▄█████▓ ██▓ ▒█████   ███▄    █ 
 ██ ▀█   █  ██  ▓██▒▓██▒    ▓██▒       ▓██   ▒▓██▒ ██▒ ▀█▒▓██░ ██▒▓  ██▒ ▓▒▓█   ▀ ▓██ ▒ ██▒   ▓█   ▀ ▒▒ █ █ ▒░▒██▀ ▀█  ▓█   ▀ ▓██░  ██▒▓  ██▒ ▓▒▓██▒▒██▒  ██▒ ██ ▀█   █ 
▓██  ▀█ ██▒▓██  ▒██░▒██░    ▒██░       ▒████ ░▒██▒▒██░▄▄▄░▒██▀▀██░▒ ▓██░ ▒░▒███   ▓██ ░▄█ ▒   ▒███   ░░  █   ░▒▓█    ▄ ▒███   ▓██░ ██▓▒▒ ▓██░ ▒░▒██▒▒██░  ██▒▓██  ▀█ ██▒
▓██▒  ▐▌██▒▓▓█  ░██░▒██░    ▒██░       ░▓█▒  ░░██░░▓█  ██▓░▓█ ░██ ░ ▓██▓ ░ ▒▓█  ▄ ▒██▀▀█▄     ▒▓█  ▄  ░ █ █ ▒ ▒▓▓▄ ▄██▒▒▓█  ▄ ▒██▄█▓▒ ▒░ ▓██▓ ░ ░██░▒██   ██░▓██▒  ▐▌██▒
▒██░   ▓██░▒▒█████▓ ░██████▒░██████▒   ░▒█░   ░██░░▒▓███▀▒░▓█▒░██▓  ▒██▒ ░ ░▒████▒░██▓ ▒██▒   ░▒████▒▒██▒ ▒██▒▒ ▓███▀ ░░▒████▒▒██▒ ░  ░  ▒██▒ ░ ░██░░ ████▓▒░▒██░   ▓██░
░ ▒░   ▒ ▒ ░▒▓▒ ▒ ▒ ░ ▒░▓  ░░ ▒░▓  ░    ▒ ░   ░▓   ░▒   ▒  ▒ ░░▒░▒  ▒ ░░   ░░ ▒░ ░░ ▒▓ ░▒▓░   ░░ ▒░ ░▒▒ ░ ░▓ ░░ ░▒ ▒  ░░░ ▒░ ░▒▓▒░ ░  ░  ▒ ░░   ░▓  ░ ▒░▒░▒░ ░ ▒░   ▒ ▒ 
░ ░░   ░ ▒░░░▒░ ░ ░ ░ ░ ▒  ░░ ░ ▒  ░    ░      ▒ ░  ░   ░  ▒ ░▒░ ░    ░     ░ ░  ░  ░▒ ░ ▒░    ░ ░  ░░░   ░▒ ░  ░  ▒    ░ ░  ░░▒ ░         ░     ▒ ░  ░ ▒ ▒░ ░ ░░   ░ ▒░
   ░   ░ ░  ░░░ ░ ░   ░ ░     ░ ░       ░ ░    ▒ ░░ ░   ░  ░  ░░ ░  ░         ░     ░░   ░       ░    ░    ░  ░           ░   ░░         ░       ▒ ░░ ░ ░ ▒     ░   ░ ░ 
         ░    ░         ░  ░    ░  ░           ░        ░  ░  ░  ░            ░  ░   ░           ░  ░ ░    ░  ░ ░         ░  ░                   ░      ░ ░           ░
         

THIS CODE WAS WRITTEN AS A PART OF A GROUP FOR 1ST YEAR OBJECT-ORIENTED PROGRAMMING.

Game: Null Fighter Exception

Null Fighter Exception (NFE) is a turn-based role-playing-game battle system loosely based on Dungeons and Dragons (DnD). NFE has the player pick a class to play as (Warrior or Bard) to fight a chosen foe (A bugbear or an evil mage). The player takes turns with the foe, by selecting 1 of 4 different options. Both the player and the foe have 4 options which differ from class to class. 
       
Quick definitions:
      AC
    It stands for "armor class". This is the number used to determine if the entity is hit or not. Example: player attacks the monster. The monster's AC is 15, so if the player "rolls" a 14 or lower, the player misses. If the player "rolls" a 15 or higher, they hit the monster.
      Hex
    It is an effect that the evil mage can give the player. It stops the player from healing.
      HP
    It stands for "health points". This is what is used to measure how alive or close to death the monster or player is.
      Advantage
    Advantage means that you roll to hit twice and take the higher roll. Disadvantage means you roll twice and take the lower roll.
      roll xdy (e.g. roll a 2d6)
    In DnD, different shaped/sided dice was used to determine how actions played out. A shorthand way to ask to a roll a specific dice was needed and therefore "roll a 2d6". The D stand for dice, therefore "roll a 2d6" -> "roll 2 6-sided dice" 
      
