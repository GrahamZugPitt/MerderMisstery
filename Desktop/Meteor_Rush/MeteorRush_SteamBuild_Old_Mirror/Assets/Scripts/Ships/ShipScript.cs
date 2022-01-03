using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using TMPro;
using System;
using Mirror;
using UnityEngine.Tilemaps;
using System.Linq;

public class ShipScript : NetworkBehaviour
{
    public enum Spaces{
        METEOR,
        STRUCTURE,
        BLANK,
        BASE,
        GOAL
    }
    enum Team
    {
        RED,
        BLUE,
        NONE
    }

    public Vector3 origin;
    public GameObject text_display; //this is for displaying the meteor value, but not cargo or movement
    public GameObject movementSquare;
    public GameObject enemySquare;
    public GameObject[] squares;
    public Vector3[] locations;
    public Vector3[] shipsInRange;
    public Vector3 dest;
    public int frameCount = 100;
    public int length = 0;
    public int spaceType;
    public GameObject[] buildings;
    public int building_cycle = 0;
    public int player_number = -1;
    public Sprite miningSprite;
    public Sprite cashingSprite;

    public Sprite activated;
    public Sprite deactivated;
    public Sprite blue_activated;
    public Sprite blue_deactivated;
    public Sprite big_red;
    public Sprite big_blue;
    public Sprite factory_sprite;

    public bool added_to_stack;
    public bool hasChosenKickingTarget = false;
    public bool kickedByFriendly = false;
    public bool isLiterallyMoving;
    public bool isMoving;
    public bool isKicking;
    public bool isKickable = true;
    public bool isBeingKicked = false;
    public bool isMining = false;
    public bool isDeactivated = false;
    public bool cashingIn = false;
    public bool specialMovement = false;
    public bool specialCargo = false;
    public bool specialPrice = false;
    public bool hasAbility = false;
    public bool isShowingRangeToEnemy = false;
    public bool displayingInterruptedByKick = false;
    public bool temp_movement = false;
    public bool in_graveyard = false;

    public int movement;
    public int temp_movement_number;
    public int cargoSpace;
    public int miningSpeed;
    public int metalCount;
    public int platCount = 0;
    public int attackScore = 2;
    public int identifier; 

    public GameObject meteor_text;
    public GameObject text_display_prefab; //this is for displaying cargo and movement, but not meteor value
    public GameObject movement_value_display;
    public GameObject cargo_value_display;
    public GameObject mining_symbol_prefab;
    public GameObject mining_symbol;
    public GameObject building_symbol;
    public GameObject enemy_movement_square_prefab;

    public float movementPerSecond = 5.0f;

    [TextArea]
    public string description;

    public virtual bool isValid(Vector3 original, Vector3 dest)
    {
        float xDist = Math.Abs(dest.x - original.x);
        float yDist = Math.Abs(dest.y - original.y);
        float totDist = xDist + yDist;
        return totDist <= movement;
        
    }


    public virtual void ResetEnemySquares()
    {/*
        BaseScript enemyBase;
        if (hasAuthority)
        {
            enemyBase = GetComponentInParent<BoardScript>().bases[(GetComponentInParent<BaseScript>().player_number + 1) % 2].GetComponent<BaseScript>();
        }
        else
        {
            enemyBase = GetComponentInParent<BoardScript>().bases[GetComponentInParent<BaseScript>().player_number].GetComponent<BaseScript>();
        }
        ShipScript[] ships = enemyBase.GetComponentsInChildren<ShipScript>();

        for (int i = 0; i < ships.Length; i++)
        {
            if (ships[i].isShowingRangeToEnemy)
            {
                ships[i].ShowNextTurnRange();
            }
        }*/
    }

    public virtual void ShowNextTurnRange()
    {/*
        isShowingRangeToEnemy = true;
        if(!isDeactivated)
            showRange(movement, enemy_movement_square_prefab, enemy_movement_square_prefab);
     */
    }

    public void KillSquares()
    {
        for (int i = 0; i < squares.Length; i++)
        {
            if (squares[i] != null)
            {
                GetComponentInParent<BaseScript>().enemy_squares_displayed[(int)squares[i].transform.position.x, (int)squares[i].transform.position.y] = false;
                Destroy(squares[i]);
            }
        }
        if (isBeingKicked)
        {
            if (isShowingRangeToEnemy)
            {
                displayingInterruptedByKick = true;
            }
            isShowingRangeToEnemy = false;
            //ResetEnemySquares();
        }
    }

    public virtual void showRange(int dist, GameObject square, GameObject enemy)
    {
        KillSquares();
        Tilemap tilemap = GetComponentInParent<Transform>().GetComponentInParent<Tilemap>();
        Vector3[] takenPositions = GetComponentInParent<Transform>().GetComponentInParent<BoardScript>().shipPositions();
        squares = new GameObject[(dist * 2 + 1) * (dist * 2 + 1)];
        locations = new Vector3[(dist * 2 + 1) * (dist * 2 + 1)];
        showLegalMoves(dist, square, enemy);
    }
    public virtual void showLegalMoves(int dist, GameObject square, GameObject enemy)
    {
        squares = new GameObject[(dist * 2 + 1) * (dist * 2 + 1)];
        locations = new Vector3[(dist * 2 + 1) * (dist * 2 + 1)];
        shipsInRange = new Vector3[(dist * 2 + 1) * (dist * 2 + 1)];
        Vector3[] units = { Vector3.up, Vector3.right, Vector3.down, Vector3.left };

        for(int k = 0; k < locations.Length; k++)
        {
            locations[k] = new Vector3(-1000, -1000, -1000);
            shipsInRange[k] = new Vector3(-1000, -1000, -1000);
        }
        int i = 0;
        int m = 0;
        int b = 0;
        bool first = true;
        bool any = false;
        do
        {
            if (first)
            {
                for (int a = 0; a < units.Length; a++)
                {
                    if (freeSpace(transform.position + units[a]))
                    {
                        locations[i] = transform.position + units[a];
                        squares[i] = Instantiate(square, locations[i], Quaternion.identity);
                        squares[i].GetComponent<movementSquare>().dist = 1;
                        i++;
                        any = true;
                    }
                }
                first = false;
            }

            for (int j = 0; j < units.Length; j++)
            {

                if (any && dist >= squares[m].GetComponent<movementSquare>().dist + 1 && !locations.Contains(locations[m] + units[j]) && freeSpace(locations[m] + units[j])) //&& free(locations[m] + units[j]))
                {
                    locations[i] = locations[m] + units[j];
                    squares[i] = Instantiate(square, locations[i], Quaternion.identity);
                    squares[i].GetComponent<movementSquare>().dist = squares[m].GetComponent<movementSquare>().dist + 1;
                    i++;
                }

                if (any && dist >= squares[m].GetComponent<movementSquare>().dist + 1 && !shipsInRange.Contains(locations[m] + units[j]) && spaceTakenByShip(locations[m] + units[j]) && GetComponentInParent<BoardScript>().GetShipByPosition(locations[m] + units[j]).isKickable && attackScore > 0 && !isBeingKicked && !isEnemyBase(locations[m] + units[j])) //&& free(locations[m] + units[j]))
                {
                    shipsInRange[b] = locations[m] + units[j];
                    b++;
                }
            }
            m++;
        } while (any && !locations[m].Equals(new Vector3(-1000, -1000, -1000)));

        for(int k = 0; k < b; k++){
                locations[i] = shipsInRange[k];
                squares[i] = Instantiate(enemy, locations[i], Quaternion.identity);
                i++;
        }

        for (int c = 0; c < units.Length; c++)
        {
            if (
                !locations.Contains(transform.position + units[c]) 
                && spaceTakenByShip(transform.position + units[c]) 
                && GetComponentInParent<BoardScript>().GetShipByPosition(transform.position + units[c]).isKickable 
                && attackScore > 0
                && !isBeingKicked)
            {
                locations[i] = transform.position + units[c];
                squares[i] = Instantiate(enemy, locations[i], Quaternion.identity);
                shipsInRange[b++] = locations[i++];
            }
        }
        for (int j = 0; j < i; j++)
        {
            squares[j].GetComponent<movementSquare>().ship = transform.GetComponent<ShipScript>();
            if (GetComponentInParent<BaseScript>().enemy_squares_displayed[(int)squares[j].transform.position.x, (int)squares[j].transform.position.y] && isShowingRangeToEnemy)
            {
                Destroy(squares[j]);
                squares[j] = null;
            }
            if(squares[j] != null)
                GetComponentInParent<BaseScript>().enemy_squares_displayed[(int)squares[j].transform.position.x, (int)squares[j].transform.position.y] = true;
        }
        length = i;
    }

    public virtual bool isEnemyBase(Vector3 pos)
    {
        GameObject[] bases = GetComponentInParent<BoardScript>().bases;
        for (int i = 0; i < bases.Length; i++)
        {
            if (i != GetComponentInParent<BaseScript>().player_number && bases[i].transform.position.Equals(pos))
            {
                return true;
            }
        }
        return false;
    }

    public virtual bool freeSpace(Vector3 pos)
    {
        if(IsKickingShip().x != -1000 && IsKickingShip() == pos)
        {
            return true;
        }
        Tilemap tilemap = GetComponentInParent<Transform>().GetComponentInParent<Tilemap>();
        Vector3[] takenPositions = GetComponentInParent<Transform>().GetComponentInParent<BoardScript>().shipPositions();
        if (isEnemyBase(pos))
        {
            return false;
        }
        if (
            tilemap.HasTile(new Vector3Int((int)pos.x, (int)pos.y, 0)) 
            && (pos.x - transform.position.x != 0 || pos.y - transform.position.y != 0) 
            && (!GetComponentInParent<Transform>().GetComponentInParent<BoardScript>().shipPositions().Contains(pos) || (isShowingRangeToEnemy && GetComponentInParent<BoardScript>().GetShipByPosition(pos).GetComponentInParent<BaseScript>().player_number == GetComponentInParent<BaseScript>().player_number && !GetComponentInParent<BoardScript>().GetShipByPosition(pos).isDeactivated)))
            return true;
        return false;
    }

    public Vector3 IsKickingShip()
    {
        for(int i = 0; i < GetComponentInParent<BoardScript>().bases.Length; i++)
        {
            if(GetComponentInParent<BoardScript>().bases[i].GetComponent<BaseScript>().kicking_ship != null)
            {
                return GetComponentInParent<BoardScript>().bases[i].GetComponent<BaseScript>().kicking_ship.transform.position;
            }
        }
        return new Vector3(-1000, -1000, -1000);
    }

    public virtual bool spaceTakenByShip(Vector3 pos)
    {
        if (isEnemyBase(pos))
        {
            return false;
        }
        Vector3[] takenPositions = GetComponentInParent<Transform>().GetComponentInParent<BoardScript>().shipPositions();
        if (GetComponentInParent<Transform>().GetComponentInParent<BoardScript>().shipPositions().Contains(pos) && (pos.x - transform.position.x != 0 || pos.y - transform.position.y != 0))
        {
            return true;
        }
        return false;
    }

    public virtual int distanceFromOrigin(Vector3 vector)
    {
        int dist = (int)Math.Abs((vector.x - transform.position.x)) + (int)Math.Abs((vector.y - transform.position.y));
        return dist;
    }

    public void StopAllMovement()
    {
        GetComponentInParent<BaseScript>().kicking_ship = null;
        for (int i = 0; i < GetComponentInParent<BoardScript>().player_count; i++)
            GetComponentInParent<BoardScript>().GetComponentsInChildren<BaseScript>()[i].StopMovement();
    }

    /*public void OnMouseDown()
    {
        if (isMoving)
        {
            StopAllMovement();
            return;
        }
        StopAllMovement();

        if ((isMining && !isBeingKicked) || !hasAuthority || isLiterallyMoving || !GetComponentInParent<BaseScript>().my_turn || isDeactivated || isMining) // || GetComponentInParent<BoardScript>().moving_ship_semaphore != 0 || GetComponentInParent<BoardScript>().kicked_ship_semaphore != 0)
        {
            return;
        }
        origin = new Vector3(transform.position.x, transform.position.y, transform.position.z);
        if (GetComponentInParent<BaseScript>().has_a_moving_ship)
        {
            for(int i = 0; i < GetComponentInParent<BoardScript>().player_count; i++)
                GetComponentInParent<BoardScript>().GetComponentsInChildren<BaseScript>()[i].StopMovement();
        }
        GetComponentInParent<BoardScript>().StopShopping();
        if ((movement == 0 && !temp_movement) || (temp_movement && temp_movement_number == 0))
        {
            return;
        }
        GetComponentInParent<BaseScript>().has_a_moving_ship = true;
        isMoving = true;
        if (temp_movement)
        {
            showRange(temp_movement_number, movementSquare, enemySquare);
            return;
        }
        showRange(movement, movementSquare, enemySquare);
    }*/

    public virtual void StopMoving()
    {
        hasChosenKickingTarget = false;
        isMoving = false;
        if (isKicking)
        {
            GetComponent<BoxCollider2D>().enabled = true;
            isKicking = false;
        }
        isBeingKicked = false;
        GetComponentInParent<BaseScript>().has_a_moving_ship = false;
        for (int j = 0; j < length; j++)
            if (squares[j] != null && !isShowingRangeToEnemy)
            {
                GetComponentInParent<BaseScript>().enemy_squares_displayed[(int)squares[j].transform.position.x, (int)squares[j].transform.position.y] = false;
                Destroy(squares[j]);
            }
        return;
    }


    public void Update()
    {

        if (in_graveyard)
        {
            return;
        }

        if (isKicking && GetComponentInParent<BoardScript>().GetShipByPosition(dest) == null)
        {
            CmdStopMining();
            CmdMove(dest);
            GetComponent<BoxCollider2D>().enabled = true;
            isKicking = false;
        }

        if (isLiterallyMoving && !dest.Equals(transform.position))
        {
            //UpdateSprite();
            transform.position = Vector3.MoveTowards(transform.position, dest, movementPerSecond * Time.deltaTime);
            return;
        }

        if(isLiterallyMoving && dest.Equals(transform.position))
        {
            OnArrival();
            if (findSpaceType() == (int)Spaces.GOAL)
            {
                CashIn(player_number);
            }
            GetComponentInParent<BaseScript>().kicking_ship = null; //Really bad. Works though. Logic is this: if a normal ship is moving it's whatever. If a kicking ship stops, this needs to be nullified
            if (GetComponentInParent<BaseScript>().player_number != GetComponentInParent<BoardScript>().player_turn)
            {
                GetComponentInParent<BoardScript>().kicked_ship_semaphore--;
            }
            isLiterallyMoving = false;
            GetComponentInParent<BoardScript>().moving_ship_semaphore--;
            GameObject structure_o = GetComponentInParent<BoardScript>().GetStructureByPosition(dest);
            Structure structure = null;
            if(structure_o != null)
            {
                structure = structure_o.GetComponent<Structure>();
            }

            if(structure != null && GetComponentInParent<BaseScript>().my_turn && structure.player_number != GetComponentInParent<BaseScript>().player_number)
            {
                structure.Deactivate();
            }

            if (structure != null)
            {
                structure.LandingFunction(this);
            }
            
            
                UpdateButton();
            
            UpdateSprite();
        }

        if (!isBeingKicked)
        {
            kickedByFriendly = false;
        }
        
    }

    public virtual void ChooseDest(Vector3 pos)
    {
        /*for (int j = 0; j < length; j++)
            if (squares[j] != null)
            {
                GetComponentInParent<BaseScript>().enemy_squares_displayed[(int)squares[j].transform.position.x, (int)squares[j].transform.position.y] = false;
                Destroy(squares[j]);
            }*/
        dest = pos;
        if (shipsInRange.Contains(dest))
        {
            isKicking = true;
            GetComponent<BoxCollider2D>().enabled = false;
            GetComponentInParent<BaseScript>().kicking_ship = this;
            CmdSetKickingShip();
            Kick();
            return;
        }
        if (!isKicking)
        {
            CmdMove(dest);
            if (isBeingKicked)
            {
                CmdKick(dest, kickedByFriendly);
                isBeingKicked = false;
                /*if (!kickedByFriendly)
                {
                    GetComponentInParent<BoardScript>().CmdUpdateMeteorCount(platCount);
                }*/
            }
            GetComponentInParent<BaseScript>().has_a_moving_ship = false;
            isMoving = false;
        }
    }

    [Command]
    public void CmdSetKickingShip()
    {
        RpcSetKickingShip();
    }

    [ClientRpc]
    public void RpcSetKickingShip()
    {
        GetComponentInParent<BaseScript>().kicking_ship = this;
    }

    public virtual void Kick()
    {
        ShipScript ship = GetComponentInParent<BoardScript>().GetShipByPosition(dest);
        for (int j = 0; j < length; j++)
            if (squares[j] != null)
            {
                GetComponentInParent<BaseScript>().enemy_squares_displayed[(int)squares[j].transform.position.x, (int)squares[j].transform.position.y] = false;
                Destroy(squares[j]);
            }
        if (ship.hasAuthority)
        {
            ship.kickedByFriendly = true;
        }
        ship.isBeingKicked = true;
        ship.showRange(attackScore, movementSquare, enemySquare);
        ship.isMoving = true;
        ship.origin = GetComponentInParent<BoardScript>().GetShipByPosition(dest).GetPosition();
        hasChosenKickingTarget = true;
    }

    public virtual void CmdSteal()
    {
        if(cargoSpace - platCount != 0 && GetComponentInParent<BoardScript>().GetShipByPosition(dest).platCount > 0)
        {
            UpdatePlatCount(platCount + 1);
        }
    }

    public virtual Vector3 GetPosition()
    {
        return transform.position;
    }

    [Command(ignoreAuthority = true)]
    public virtual void CmdKick(Vector3 finish, bool friendlyKick)
    {
        RpcKick(finish, friendlyKick);
    }

    [ClientRpc]
    public virtual void RpcKick(Vector3 finish, bool friendlyKick)
    {
        isLiterallyMoving = true;
        StopMining();
        dest = finish;
        for (int i = 0; i < GetComponentInParent<BoardScript>().bases.Length; i++) {
            GetComponentInParent<BoardScript>().bases[i].GetComponent<BaseScript>().kicking_ship = null;
        }
    }

    [Command(ignoreAuthority = true)]
    public virtual void CmdDestroy()
    {
        RpcDestroy();
    }

    [ClientRpc]
    public virtual void RpcDestroy()
    {
        Destroy();
    }


    [Command]
    public virtual void CmdMove(Vector3 finish)
    {
        RpcMove(finish);
    }
    [ClientRpc]
    public virtual void RpcMove(Vector3 finish)
    {
        isDeactivated = true;
        isLiterallyMoving = true;
        GetComponentInParent<BoardScript>().moving_ship_semaphore++;
        dest = finish;
        //UpdateSprite();
    }

    public virtual void startMining()
    {
        if (findSpaceType() == (int)Spaces.METEOR)
        {
            CmdStartMining();
        }
    }

    public virtual int findSpaceType()
    {
        return GetComponentInParent<BoardScript>().GetSpaceType(transform.position);
        
    }

    public virtual void UpdateSpaceType()
    {
        spaceType = findSpaceType();
    }

    public virtual void UpdateSprite()
    {

        UpdateSpaceType();

        GetComponent<SpriteRenderer>().sprite = isDeactivated ? deactivated : activated;

        if (!isMining)
        {
            if (mining_symbol != null) //&& GetComponentInParent<BoardScript>().GetStructureByPosition(transform.position) == null)
            {
                GetComponentInChildren<Mining_Symbol>().Destroy();
                mining_symbol = null;
            }
            return;
        }

        if (isMining && building_cycle == 0)
        {
            if (mining_symbol != null)
            {
                GetComponentInChildren<Mining_Symbol>().Destroy();
                mining_symbol = null;
            }
            mining_symbol = Instantiate(mining_symbol_prefab, transform.position + new Vector3((float).25, (float).25, 0), Quaternion.identity);
            mining_symbol.GetComponent<SpriteRenderer>().sprite = miningSprite; 
            mining_symbol.transform.SetParent(transform);
            return;
        }
        if (isMining && building_cycle == 1)
        {
            if (mining_symbol != null)
            {
                GetComponentInChildren<Mining_Symbol>().Destroy();
                mining_symbol = null;
            }
            mining_symbol = Instantiate(mining_symbol_prefab, transform.position + new Vector3((float).25, (float).25, 0), Quaternion.identity);
            mining_symbol.GetComponent<SpriteRenderer>().sprite = cashingSprite;
            mining_symbol.transform.SetParent(transform);
            return;
        }
        if (isMining)
        {
            if (mining_symbol != null)
            {
                GetComponentInChildren<Mining_Symbol>().Destroy();
                mining_symbol = null;
            }
            mining_symbol = Instantiate(mining_symbol_prefab, transform.position + new Vector3((float).25, (float).25, 0), Quaternion.identity);
            mining_symbol.GetComponent<SpriteRenderer>().sprite = buildings[building_cycle - 2].GetComponent<Structure>().construction_sprites[GetComponentInParent<BaseScript>().player_number];
            mining_symbol.transform.SetParent(transform);
            return;
        }
    }

    public virtual int getSpaceType()
    {
        return spaceType;
    }

    public virtual void OnMouseExit()
    {
        GetComponentInParent<BoardScript>().ship_tracker.GetComponent<TileTracker>().unload();
        GetComponentInParent<BoardScript>().cargo_tracker.GetComponent<TileTracker>().unload();
        GetComponentInParent<BoardScript>().background_tracker.GetComponent<TileTracker>().unload();
        added_to_stack = false;
    }

    public virtual void Mine()
    {
        int mine_value = Math.Min(getRemainingCargoSpace(), miningSpeed);
        if (isMining)
        {
            isMining = false;
            switch (building_cycle)
            {
                case 0:
                    UpdatePlatCount(attackScore + GetComponentInParent<BoardScript>().GetMeteorByPosition(transform.position).GetComponent<Meteor>().Mine(mine_value, this));
                    spaceType = (int)Spaces.BLANK;
                    break;
                //GetComponent<SpriteRenderer>().sprite = isDeactivated ? deactivated : activated; Very Strange you have to do this
                case 1:
                    spaceType = (int)Spaces.BLANK;
                    GetComponentInParent<BoardScript>().GetMeteorByPosition(transform.position).GetComponent<Meteor>().CmdDestroy();
                    float metal = GetComponentInParent<BaseScript>().avaliable_metal + GetComponentInParent<BoardScript>().M_PER_METEOR;
                    GetComponentInParent<BaseScript>().UpdateMetalCount(metal);
                    break;
                default:
                    spaceType = (int)Spaces.BLANK;
                    GameObject structure = GetComponentInParent<BoardScript>().GetStructureByPosition(transform.position);
                    GetComponentInParent<BoardScript>().GetMeteorByPosition(transform.position).GetComponent<Meteor>().CmdDestroy();
                    structure.GetComponent<Structure>().color(GetComponentInParent<BaseScript>().player_number);
                    structure.transform.SetParent(GetComponentInParent<BaseScript>().transform);
                    break;
            }
        }
        UpdateSprite();
    }

    public GameObject SpawnStructure()
    {
        GameObject structure = Instantiate(buildings[building_cycle - 2], transform.position, Quaternion.identity);
        NetworkServer.Spawn(structure);
        return structure;
    }


    public virtual void OnMouseOver()
    {

        if (!added_to_stack)
        {
            GetComponentInParent<BoardScript>().ship_tracker.GetComponent<TileTracker>().add_sprite(deactivated);
            for (int i = 0; i < platCount; i++)
            {
                GetComponentInParent<BoardScript>().cargo_tracker.GetComponent<TileTracker>().add_sprite(GetComponentInParent<BaseScript>().platinum);
            }
            added_to_stack = true;
        }

        if (Input.GetMouseButtonDown(1) && GetComponentInParent<BaseScript>().my_turn && findSpaceType() == (int)Spaces.METEOR)
        {
            if (isMining && building_cycle == 0)
            {
                CmdStopMining();
                UpdateButton();
                return;
            }

            CmdStartMining();
            
        }

        /*if (Input.GetMouseButtonDown(1) && GetComponentInParent<BaseScript>().my_turn && findSpaceType() == (int)Spaces.BASE)
        {
            float metal = GetComponentInParent<BaseScript>().avaliable_metal + GetComponentInParent<BoardScript>().M_PER_METEOR*platCount;
            GetComponentInParent<BaseScript>().CmdUpdateMetalCount(metal);
            CmdUpdatePlatCount(0);
            return;
        }*/

        if (hasAuthority && Input.GetMouseButtonDown(1) && GetComponentInParent<BaseScript>().my_turn && findSpaceType() == (int)Spaces.GOAL && GetComponentInParent<BoardScript>().GetGoalByPosition(transform.position).GetComponent<GoalSquare>().square_color == GetComponentInParent<BaseScript>().player_number) //This is KIND OF a magic number. Look for trouble here when adding more players
        {
            StopAllMovement();
            //CmdCashIn(GetComponentInParent<BoardScript>().get_player_number());
            return;
        }

        /*if (Input.GetMouseButtonDown(1) && !isDeactivated && GetComponentInParent<BaseScript>().my_turn && !(findSpaceType() == (int)Spaces.STRUCTURE) && !(findSpaceType() == (int)Spaces.METEOR) && !(findSpaceType() == (int)Spaces.BASE))
        {
            CmdDoAbility();
        }*/
    }

    [Command]
    public virtual void CmdDoAbility()
    {
        RpcDoAbility();
    }

    [ClientRpc]
    public virtual void RpcDoAbility()
    {
        DoAbility();
    }

    public virtual void DoAbility()
    {

    }

    [Command]
    public virtual void CmdCapturePlanet(int plat, int player_number)
    {
        RpcCapturePlanet(plat, player_number);
    }

    [ClientRpc]
    public virtual void RpcCapturePlanet(int plat, int player_number)
    {
        Planet planet = GetComponentInParent<BoardScript>().GetPlanetByPosition(transform.position).GetComponent<Planet>();
        UpdatePlatCount(planet.Capture(plat, player_number)); 
        if(player_number == planet.player_number)
        {
            //CmdDestroy();
        }
    }

    public virtual void StartTurnPriority()
    {

    }

    public virtual void StartTurn()
    {
        isDeactivated = false;
        hasChosenKickingTarget = false;
        Mine();
        building_cycle = 0;
        UpdateSprite();
    }

    public virtual void StartTurnDelayed()
    {

    }


    public virtual void EndTurn()
    {
        int final_dest = 0;
        for(int i = 1; i <= movement; i++)
        {
            if(GetComponentInParent<BoardScript>().GetSpaceType(transform.position + new Vector3(color_multiplier(player_number)*i, 0, 0)) == (int)Spaces.GOAL)
            {
                final_dest = i;
                break;
            }
            final_dest = i;
        }
        ChooseDest(transform.position + new Vector3(color_multiplier(player_number) * final_dest, 0, 0));
        temp_movement = false;
        isDeactivated = true;
        UpdateSprite();
    }

    public int color_multiplier(int player_number)
    {
        if(player_number == 0)
        {
            return 1;
        }
        return -1;
    }

    public virtual int getRemainingCargoSpace()
    {
        return cargoSpace - metalCount - platCount;
    }

    public virtual bool IsFull()
    {
        return getRemainingCargoSpace() == 0;
    }

    [Command]
    public virtual void CmdSetMovementDisplay(int movement)
    {
        RpcSetMovementDisplay(movement);
    }

    [ClientRpc]
    public virtual void RpcSetMovementDisplay(int movement)
    {
        movement_value_display.GetComponent<StatHolder>().UpdateValue(movement);
    }

    /*[Command]
    public virtual void CmdCashIn(int player_number)
    {
        RpcCashIn(player_number);
    }*/

    //[ClientRpc]
    public virtual void CashIn(int player_number)
    {
        
        cashingIn = true;
        GetComponentInParent<BoardScript>().cargo_tracker.GetComponent<TileTracker>().unload();
        GetComponentInParent<BoardScript>().CmdUpdateMeteorCount(0);
        //GetComponentInParent<BaseScript>().captured_platinum = GetComponentInParent<BaseScript>().captured_platinum + platCount;
        GetComponentInParent<BoardScript>().scoreBoards[(player_number + 1) % 2].GetComponent<ScoreKeeper>().AddToScore(-attackScore);
        //GetComponentInParent<BoardScript>().scoreBoards[2].GetComponent<ScoreKeeper>().DecrementShipBoard();
        UpdatePlatCount(0);
        Destroy();
    }

    public virtual void UpdatePlatCount(int newPlatValue)
    {
        /*if(meteor_text == null)
        {
            meteor_text = Instantiate(text_display, transform.position, Quaternion.identity);
            meteor_text.transform.SetParent(transform);
        }
        platCount = newPlatValue;
        if(newPlatValue == 0)
        {
            meteor_text.GetComponent<TextMeshPro>().SetText("");
            return;
        }*/
        attackScore = newPlatValue;
        cargo_value_display.GetComponent<StatHolder>().UpdateValue(newPlatValue.ToString());
    }

    [Command]
    public virtual void CmdUpdatePlatCount(int newPlatValue)
    {
        RpcUpdatePlatCount(newPlatValue);
    }

    [ClientRpc]
    public virtual void RpcUpdatePlatCount(int newPlatValue)
    {
        UpdatePlatCount(newPlatValue);
    }

    public virtual void UpdateMovement(int newMoveValue)
    {
        movement = newMoveValue;
        movement_value_display.GetComponent<StatHolder>().UpdateValue(movement);
        //ResetEnemySquares();
    }

    [Command]
    public virtual void CmdUpdateMovement(int newMoveValue)
    {
        RpcUpdateMovement(newMoveValue);
    }

    [ClientRpc]
    public virtual void RpcUpdateMovement(int newMoveValue)
    {
        UpdateMovement(newMoveValue);
    }

    /*public virtual void UpdateCargo(int newCargoValue)
    {
        cargoSpace = newCargoValue;
        cargo_value_display.GetComponent<StatHolder>().UpdateValue(cargoSpace);
        //ResetEnemySquares();
    }

    [Command]
    public virtual void CmdUpdateCargo(int newCargoValue)
    {
        RpcUpdateCargo(newCargoValue);
    }

    [ClientRpc]
    public virtual void RpcUpdateCargo(int newCargoValue)
    {
        UpdateCargo(newCargoValue);
    }*/

    [Command]
    public virtual void CmdStopMining()
    {
        RpcStopMining();
    }
    [ClientRpc]
    public virtual void RpcStopMining()
    {
        StopMining();
    }

    public void StopMining()
    {
        building_cycle = 0;
        isMining = false;
        UpdateSprite();
        UpdateButton();
    }

    [Command]
    public virtual void CmdStartMining()
    {
        RpcStartMining();
    }
    [ClientRpc]
    public virtual void RpcStartMining()
    {
        building_cycle = (1 + building_cycle) % (buildings.Length + 2);
        isMining = true;
        UpdateSprite();
        UpdateButton();
    }

    public void color()
    {
        int base_number = GetComponentInParent<BaseScript>().player_number;
        BoardScript board = GetComponentInParent<Transform>().GetComponentInParent<BoardScript>();
        if (base_number == 1) //Magic number
        {
            GetComponent<SpriteRenderer>().sprite = blue_deactivated;
            activated = blue_activated;
            deactivated = blue_deactivated;

            miningSprite = board.blue_mine;
            cashingSprite = board.blue_cash;
            return;

        }
        miningSprite = board.red_mine;
        cashingSprite = board.red_cash;
    }

    public Vector3 getPosition()
    {
        return transform.position;
    }

    public void UpdateButton()
    {
        GetComponentInParent<BoardScript>().UpdateButton();
    }

    public void Destroy()
    {
        if (cashingIn)
        {
            Destroy(gameObject);
            return;
        }
        in_graveyard = true;
        GetComponentInParent<BaseScript>().GetComponentInChildren<Graveyard>().Push(gameObject);
        isMining = false;
    }

    public virtual void OnArrival()
    {

    }

}
