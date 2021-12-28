using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;
using Mirror;
using UnityEngine.Tilemaps;
using System.Linq;

public class ShipScript : NetworkBehaviour
{
    public enum spaces{
        METEOR,
        PLANET,
        BLANK,
        BASE,
    }
    private bool isLiterallyMoving;
    private bool isLiterallyBeingKicked;
    private bool isMoving;
    private bool isKicking;
    private Vector3 origin;
    public GameObject movementSquare;
    public GameObject enemySquare;
    public GameObject[] squares;
    public Vector3[] locations;
    public Vector3[] shipsInRange;
    public Vector3 dest;
    public int frameCount = 100;
    public int length = 0;
    int timeElapsed = 0;
    int spaceType = (int)spaces.BLANK;
    public Sprite miningSprite;
    public bool isKickable = true;
    public bool willHaveActed = false;

    public Sprite has_not_acted;
    public Sprite has_acted;
    public Sprite blue_has_not_acted;
    public Sprite blue_has_acted;
    public Sprite on_asteroid;
    public Sprite on_asteroid_blue;
    public Sprite on_asteroid_has_acted;
    public Sprite on_asteroid_blue_has_acted;
    public Sprite on_base;
    public Sprite on_base_has_acted;
    public Sprite on_base_blue;
    public Sprite on_base_blue_has_acted;

    public ShipScript shipBeingKicked;
    public bool added_to_stack;
    public bool wasKicked = false;
    public bool hasChosenKickingTarget = false;
    public bool kickedWhileMining = false;

    public bool isBeingKicked = false;
    public bool isMining = false;
    public bool hasActed = false;
    public bool cashingIn = false;
    public GameObject objectAtLocation;
    public int movement;
    public int cargoSpace;
    public int attack;
    public int miningSpeed;
    public int metalCount;
    public int platCount;
    public int kickingDistance = 2;

    public virtual bool isValid(Vector3 original, Vector3 dest)
    {
        float xDist = Math.Abs(dest.x - original.x);
        float yDist = Math.Abs(dest.y - original.y);
        float totDist = xDist + yDist;
        return totDist <= movement;
        
    }

    /*public virtual void showRange(int area)
    {
        Tilemap tilemap = GetComponentInParent<Transform>().GetComponentInParent<Tilemap>();
        int k = 0;
        Vector3[] takenPositions = GetComponentInParent<Transform>().GetComponentInParent<BoardScript>().shipPositions();
        squares = new GameObject[(area * 2 + 1)* (area * 2 + 1)];
        locations = new Vector3[(area * 2 + 1) * (area * 2 + 1)];
        for (int i = -area; i <= area; i++)
            for (int j = -Math.Abs(area - Math.Abs(i)); j <= Math.Abs(area - Math.Abs(i)); j++){
                if (tilemap.HasTile(new Vector3Int((int)transform.position.x + i, (int)transform.position.y + j, 0)) && (i != 0 || j != 0))
                {
                    bool taken = false;
                    for(int a = 0; a < takenPositions.Length; a++)
                    {
                        if(takenPositions[a].x == ((int)transform.position.x + i) && takenPositions[a].y == ((int)transform.position.y + j))
                            taken = true;
                    }
                    if (!taken) //This is inefficient, maybe fix it later
                    {
                        squares[k] = GameObject.Instantiate(movementSquare as GameObject);
                        squares[k].transform.position = transform.position + new Vector3(i, j, 0);
                        locations[k] = squares[k].transform.position;
                        k++;
                    }
                }

            }
        length = k;
    }*/

    public virtual void showRange(int dist)
    {
        Tilemap tilemap = GetComponentInParent<Transform>().GetComponentInParent<Tilemap>();
        Vector3[] takenPositions = GetComponentInParent<Transform>().GetComponentInParent<BoardScript>().shipPositions();
        squares = new GameObject[(dist * 2 + 1) * (dist * 20 + 1)];
        locations = new Vector3[(dist * 2 + 1) * (dist * 20 + 1)];
        showLegalMoves(dist);
    }

    public int test = 0;
    public virtual void showLegalMoves(int dist)
    {
        squares = new GameObject[(dist * 2 + 1) * (dist * 20 + 1)];
        locations = new Vector3[(dist * 20 + 1) * (dist * 20 + 1)];
        shipsInRange = new Vector3[(dist * 20 + 1) * (dist * 20 + 1)];
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
                        squares[i] = Instantiate(movementSquare, locations[i], Quaternion.identity);
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
                    squares[i] = Instantiate(movementSquare, locations[i], Quaternion.identity);
                    squares[i].GetComponent<movementSquare>().dist = squares[m].GetComponent<movementSquare>().dist + 1;
                    i++;
                }

                if (any && dist >= squares[m].GetComponent<movementSquare>().dist + 1 && !shipsInRange.Contains(locations[m] + units[j]) && spaceTakenByShip(locations[m] + units[j]) && !isBeingKicked) //&& free(locations[m] + units[j]))
                {
                    shipsInRange[b] = locations[m] + units[j];
                    b++;
                }
            }
            m++;
        } while (any && !locations[m].Equals(new Vector3(-1000, -1000, -1000)));

        for(int k = 0; k < b; k++){
            locations[i] = shipsInRange[k];
            squares[i] = Instantiate(enemySquare, locations[i], Quaternion.identity);
            i++;
        }

        for (int c = 0; c < units.Length; c++)
        {
            if (!locations.Contains(transform.position + units[c]) && spaceTakenByShip(transform.position + units[c]) && !isBeingKicked)
            {
                locations[i] = transform.position + units[c];
                squares[i] = Instantiate(enemySquare, locations[i], Quaternion.identity);
                shipsInRange[b++] = locations[i++];
            }
        }

        length = i;
    }

    public virtual bool freeSpace(Vector3 pos)
    {
        Debug.Log(transform.position);
        Tilemap tilemap = GetComponentInParent<Transform>().GetComponentInParent<Tilemap>();
        Vector3[] takenPositions = GetComponentInParent<Transform>().GetComponentInParent<BoardScript>().shipPositions();
        if (tilemap.HasTile(new Vector3Int((int)pos.x, (int)pos.y, 0)) && (pos.x - transform.position.x != 0 || pos.y - transform.position.y != 0) && !GetComponentInParent<Transform>().GetComponentInParent<BoardScript>().shipPositions().Contains(pos))
            return true;
        return false;
    }

    public virtual bool spaceTakenByShip(Vector3 pos)
    {
        Vector3[] takenPositions = GetComponentInParent<Transform>().GetComponentInParent<BoardScript>().shipPositions();
        if (GetComponentInParent<Transform>().GetComponentInParent<BoardScript>().shipPositions().Contains(pos) && (pos.x - transform.position.x != 0 || pos.y - transform.position.y != 0))
            return true;
        return false;
    }

    public virtual int distanceFromOrigin(Vector3 vector)
    {
        int dist = (int)Math.Abs((vector.x - transform.position.x)) + (int)Math.Abs((vector.y - transform.position.y));
        return dist;
    }

    public virtual void OnMouseDown()
    {
        if (isBeingKicked)
        {
            StopMoving();
            wasKicked = false; //Maybe remove this later
            kickedWhileMining = false;
            return;
        }

        if (!hasAuthority || isLiterallyMoving || !GetComponentInParent<BaseScript>().my_turn || hasActed || isMining) //TODO: Add "Is owned by this player" check
        {
            return;
        }
        if (isMoving)
        {
            StopMoving();
            return;
        }
        origin = new Vector3(transform.position.x, transform.position.y, transform.position.z);
        if (GetComponentInParent<BaseScript>().has_a_moving_ship)
            GetComponentInParent<BaseScript>().StopMovement();
        GetComponentInParent<BaseScript>().has_a_moving_ship = true;
        GetComponentInParent<BoardScript>().StopShopping();
        isMoving = true;
        showRange(movement);
    }

    public virtual void StopMoving()
    {
        hasChosenKickingTarget = false;
        isMoving = false;
        isKicking = false;
        GetComponentInParent<BaseScript>().has_a_moving_ship = false;
        for (int j = 0; j < length; j++)
            if (squares[j] != null)
                Destroy(squares[j]);
        return;
    }


    public virtual void Update()
    {
        if (isKicking && GetComponentInParent<BoardScript>().FindShipByPosition(dest) != null)
        {
            if(GetComponentInParent<BoardScript>().FindShipByPosition(dest).wasKicked == false)
            {
                StopMoving();
            }
            return;
        }
        if (isKicking && GetComponentInParent<BoardScript>().FindShipByPosition(dest) == null)
        {
            isKicking = false;
        }

        if (!hasAuthority && !isBeingKicked && !isLiterallyBeingKicked)
            return;
        if (isLiterallyMoving && !isLiterallyBeingKicked && timeElapsed < frameCount)
        {
            CmdMove(origin, dest, timeElapsed, frameCount);
            timeElapsed++;
            return;
        }
        if (isLiterallyMoving && isLiterallyBeingKicked && timeElapsed < frameCount)
        {
            CmdKick(origin, dest, timeElapsed, frameCount);
            timeElapsed++;
            return;
        }
        //if(transform.position.x % 1 != 0 || transform.position.x % 1 != 0 || transform.position.x % 1 != 0)
        if (hasAuthority)
        {
            CmdSnap();// This results in a lot of calls, but a lot of checks implicitly depend on it so uhhh idk lol
        }
        if (willHaveActed)
        {
            willHaveActed = false;
            CmdAct();
        }
            timeElapsed = 0;
            isLiterallyMoving = false;
        isLiterallyBeingKicked = false;
        if (isMoving)
        {
            if (squares != null)
            {
                for (int i = 0; i < length; i++)
                {
                    if (squares[i] == null && !isKicking)
                    {
                        if (!hasChosenKickingTarget || (hasChosenKickingTarget && isBeingKicked))
                        {
                            dest = locations[i];
                        }
                        if (!hasChosenKickingTarget && shipsInRange.Contains(dest))
                        {
                            isKicking = true;
                            Kick();
                            return;
                        }
                        if (!isKicking)
                        {
                            willHaveActed = true;
                            isLiterallyMoving = true;
                            if (isBeingKicked)
                            {
                                isBeingKicked = false;
                                isLiterallyBeingKicked = true;
                            }
                            GetComponentInParent<BaseScript>().has_a_moving_ship = false;
                            isMoving = false;
                            for (int j = 0; j < length; j++)
                                if (squares[j] != null)
                                    Destroy(squares[j]);
                        }
                    }
                }
            }
        }
    }

    public virtual void Kick()
    {
        for (int j = 0; j < length; j++)
            if (squares[j] != null)
                Destroy(squares[j]);
        GetComponentInParent<BoardScript>().FindShipByPosition(dest).isBeingKicked = true;
        GetComponentInParent<BoardScript>().FindShipByPosition(dest).showRange(kickingDistance);
        GetComponentInParent<BoardScript>().FindShipByPosition(dest).isMoving = true;
        GetComponentInParent<BoardScript>().FindShipByPosition(dest).wasKicked = true;
        GetComponentInParent<BoardScript>().FindShipByPosition(dest).origin = GetComponentInParent<BoardScript>().FindShipByPosition(dest).GetPosition();
        hasChosenKickingTarget = true;
    }

    public virtual void CmdSteal()
    {
        if(cargoSpace - platCount != 0 && GetComponentInParent<BoardScript>().FindShipByPosition(dest).platCount > 0)
        {
            platCount++;
        }
    }

    public virtual Vector3 GetPosition()
    {
        return transform.position;
    }

    [Command(ignoreAuthority = true)]
    public virtual void CmdKick(Vector3 start, Vector3 finish, int time, int frames)
    {
        RpcKick(start, finish, time, frames);
    }

    [Command (ignoreAuthority = true)]
    public virtual void CmdAct()
    {
        RpcAct();
    }
    [ClientRpc]
    public virtual void RpcAct()
    {
        hasActed = true;
    }


    [Command]
    public virtual void CmdMove(Vector3 start, Vector3 finish, int time, int frames)
    {
        RpcMove(start, finish, time, frames);
    }
    [ClientRpc]
    public virtual void RpcMove(Vector3 start, Vector3 finish, int time, int frames)
    {
        transform.position = Vector3.Lerp(start, finish, (float)time / (float)frames);
    }

    [ClientRpc]
    public virtual void RpcKick(Vector3 start, Vector3 finish, int time, int frames)
    {
        if (platCount > 0)
        {
            platCount = 0;
        }
        if (isMining)
        {
            kickedWhileMining = true;
        }
        transform.position = Vector3.Lerp(start, finish, (float)time / (float)frames);
    }


    [Command]
    public virtual void CmdSnap()
    {
        RpcSnap();
    }
    [ClientRpc]
    public virtual void RpcSnap()
    {
        transform.position = new Vector2((float)Math.Round(transform.position.x), (float)Math.Round(transform.position.y));
        findSpaceType();
    }

    public virtual void DoAbility()
    {
        Debug.Log("Doing Ability");
        return;
    }

    public virtual void startMining()
    {
        if (findSpaceType() == (int)spaces.METEOR)
        {
            CmdStartMining();
        }
    }

    public virtual int findSpaceType()
    {
        Debug.Log("I get here");
        GameObject[] meteors = GetComponentInParent<BoardScript>().meteors;
        GameObject[] bases = GetComponentInParent<BoardScript>().bases;
        for (int i = 0; i < meteors.Length; i++)
        {
            if (meteors[i] != null && transform.position.Equals(meteors[i].transform.position))
            {
                //Debug.Log("false meteor");
                if (hasAuthority)
                    CmdUpdateLocationObject(i, (int)spaces.METEOR);
                return (int)spaces.METEOR;
            }
        }
        for (int i = 0; i < bases.Length; i++)
        {
            if (transform.position.Equals(bases[i].transform.position))
            {
                if (hasAuthority)
                    CmdUpdateLocationObject(i, (int)spaces.BASE);
                return (int)spaces.BASE;
            }
        }
        isMining = false;
        if(hasActed)
            GetComponent<SpriteRenderer>().sprite = has_acted;
        if (!hasActed)
            GetComponent<SpriteRenderer>().sprite = has_not_acted;
        //CmdUpdateLocationObject(0, (int)spaces.BLANK);
        return (int)spaces.BLANK;
        
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
            switch (findSpaceType())
            {
                case (int)spaces.METEOR:
                    platCount = platCount + objectAtLocation.GetComponent<Meteor>().mineValue(mine_value);
                    objectAtLocation.GetComponent<Meteor>().Mine(mine_value);
                    break;
            }
        }
        findSpaceType();
        return;
    }

    public virtual void OnMouseOver()
    {
        if (!added_to_stack)
        {
            GetComponentInParent<BoardScript>().ship_tracker.GetComponent<TileTracker>().add_sprite(has_acted);
            if(objectAtLocation != null)
                GetComponentInParent<BoardScript>().background_tracker.GetComponent<TileTracker>().add_sprite(objectAtLocation.GetComponent<SpriteRenderer>().sprite);
            for (int i = 0; i < platCount; i++)
            {
                GetComponentInParent<BoardScript>().cargo_tracker.GetComponent<TileTracker>().add_sprite(GetComponentInParent<BaseScript>().platinum);
            }
            added_to_stack = true;
        }

        if (Input.GetMouseButtonDown(1) && GetComponentInParent<BaseScript>().my_turn && findSpaceType() == (int)spaces.METEOR && !IsFull())
        {
            if (isMining)
            {
                CmdStopMining();
                return;
            }
            startMining();
        }

        if (Input.GetMouseButtonDown(1) && GetComponentInParent<BaseScript>().my_turn && findSpaceType() == (int)spaces.BASE)
        {
            if(hasAuthority)
                CmdCashIn(GetComponentInParent<BoardScript>().get_player_number());
        }
    }

    public virtual void StartTurn()
    {
        hasChosenKickingTarget = false;
        Mine();
        hasActed = kickedWhileMining;
        kickedWhileMining = false;
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
    public void CmdUpdateLocationObject(int i, int type)
    {
        RpcUpdateLocationObject(i, type);
    }

    [ClientRpc]
    public void RpcUpdateLocationObject(int i, int type)
    {
        if (type == (int)spaces.METEOR)
        {
            objectAtLocation = GetComponentInParent<BoardScript>().meteors[i]; //update this when you add planets to include planets
            if (!isMining)
            {
                if (hasActed)
                {
                    GetComponent<SpriteRenderer>().sprite = on_asteroid_has_acted;
                    return;
                }
                GetComponent<SpriteRenderer>().sprite = on_asteroid;
                return;
            }
        }
        if(type == (int)spaces.BASE)
        {
            objectAtLocation = GetComponentInParent<BoardScript>().bases[i];
            if (hasActed)
            {
                GetComponent<SpriteRenderer>().sprite = on_base_has_acted;
                return;
            }
            GetComponent<SpriteRenderer>().sprite = on_base;
            return;
        }
        //objectAtLocation = null;
    }

    [Command]
    public virtual void CmdCashIn(int player_number)
    {
        RpcCashIn(player_number);
    }

    [ClientRpc]
    public virtual void RpcCashIn(int player_number)
    {
        cashingIn = true;
        GetComponentInParent<BoardScript>().cargo_tracker.GetComponent<TileTracker>().unload();
        if(hasAuthority)
            GetComponentInParent<BoardScript>().scoreBoards[player_number].GetComponent<ScoreKeeper>().CmdAddToScore(platCount); //This is a weird way to do this
        platCount = 0;
    }

    [Command]
    public virtual void CmdStopMining()
    {
        RpcStopMining();
    }
    [ClientRpc]
    public virtual void RpcStopMining()
    {
        isMining = false;
        findSpaceType();
    }
    [Command]
    public virtual void CmdStartMining()
    {
        RpcStartMining();
    }
    [ClientRpc]
    public virtual void RpcStartMining()
    {
        isMining = true;
        GetComponent<SpriteRenderer>().sprite = miningSprite;
    }

    public void color()
    {
        int base_number = GetComponentInParent<BaseScript>().player_number;
        BoardScript board = GetComponentInParent<Transform>().GetComponentInParent<BoardScript>();
        if (base_number == 1) //Magic number
        {
            GetComponent<SpriteRenderer>().sprite = blue_has_not_acted;
            has_not_acted = blue_has_not_acted;
            has_acted = blue_has_acted;
            on_asteroid = on_asteroid_blue;
            on_asteroid_has_acted = on_asteroid_blue_has_acted;
            on_base = on_base_blue;
            on_base_has_acted = on_base_blue_has_acted;

            miningSprite = board.blue_mine;
            return;

        }
        miningSprite = board.red_mine;
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
        Destroy(gameObject);
    }
}
