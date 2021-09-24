using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Mirror;
using UnityEngine.SceneManagement;

public class EndTurnButton : NetworkBehaviour
{
    public Sprite EndTurnOn;
    public Sprite EndTurnOff;
    public Sprite EndTurnOnYellow;
    public Sprite EndTurnOffYellow;
    public Sprite EndTurnOnRed;
    public Sprite EndTurnOffRed;
    public Sprite MainMenuOn;
    public Sprite MainMenuOff;
    public Sprite NotMyTurn;

    public GameObject ship;
    public GameObject meteor;
    public GameObject buying;

    public GameObject ship_prefab;
    public GameObject meteor_prefab;
    public GameObject buying_prefab;

    public bool game_over = false;

    public void OnMouseOver()
    {
        bool[] things_left = GetComponentInParent<BoardScript>().bases[GetComponentInParent<BoardScript>().get_player_number()].GetComponent<BaseScript>().ThingsLeftToDo();

        if (game_over)
        {
            transform.GetComponent<SpriteRenderer>().sprite = MainMenuOn;
            return;
        }
        if (!transform.GetComponentInParent<BoardScript>().my_turn())
        {
            transform.GetComponent<SpriteRenderer>().sprite = NotMyTurn;
            return;
        }

        for (int i = 0; i < things_left.Length; i++)
        {
            if (things_left[0])
            {
                transform.GetComponent<SpriteRenderer>().sprite = EndTurnOnRed;
                return;
            }
            if (things_left[i])
            {
                transform.GetComponent<SpriteRenderer>().sprite = EndTurnOnYellow;
                return;
            }
        }
        transform.GetComponent<SpriteRenderer>().sprite = EndTurnOn;
    }

    public void OnMouseDown()
    {
        if (game_over)
        {
            SceneManager.LoadScene(SceneManager.GetActiveScene().buildIndex - 1);
        }

        if (transform.GetComponentInParent<BoardScript>().my_turn())
        {
            transform.GetComponentInParent<BoardScript>().Cmd_update_turn();
            transform.GetComponent<SpriteRenderer>().sprite = NotMyTurn;
        }
    }

    public void OnMouseExit()
    {
        UpdateButton();
    }

    public void EndGame()
    {
        game_over = true;
        UpdateButton();
    }

    public void UpdateButton()
    {
        if (game_over)
        {
            transform.GetComponent<SpriteRenderer>().sprite = MainMenuOff;
            return;
        }
        if (!transform.GetComponentInParent<BoardScript>().my_turn())
        {
            transform.GetComponent<SpriteRenderer>().sprite = NotMyTurn;
            return;
        }


        bool[] things_left = GetComponentInParent<BoardScript>().bases[GetComponentInParent<BoardScript>().get_player_number()].GetComponent<BaseScript>().ThingsLeftToDo();



        /*if (things_left[0])
        {
            ship = Instantiate(ship_prefab, new Vector3((float)13.2, (float)4.5, 0), Quaternion.identity);
            ship.transform.SetParent(transform);
        }

        if (things_left[1])
        {
            meteor = Instantiate(meteor_prefab, new Vector3((float)13.7, (float)4.5, 0), Quaternion.identity);
            meteor.transform.SetParent(transform);
        }

        if (things_left[2])
        {
            buying = Instantiate(buying_prefab, new Vector3((float)14.2, (float)4.5, 0), Quaternion.identity);
            buying.transform.SetParent(transform);
        }*/

        for (int i = 0; i < things_left.Length; i++)
        {
            if (things_left[0])
            {
                transform.GetComponent<SpriteRenderer>().sprite = EndTurnOffRed;
                return;
            }
            if (things_left[i])
            {
                transform.GetComponent<SpriteRenderer>().sprite = EndTurnOffYellow;
                return;
            }
        }
        transform.GetComponent<SpriteRenderer>().sprite = EndTurnOff;
    }

}
