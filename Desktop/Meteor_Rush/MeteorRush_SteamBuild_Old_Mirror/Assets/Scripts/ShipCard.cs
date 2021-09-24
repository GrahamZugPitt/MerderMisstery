using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using TMPro;

public class ShipCard : MonoBehaviour
{
    public GameObject text_display_prefab;
    public GameObject ship_sprite_display;

    public void SetShip(GameObject ship, GameObject priceDisplay, GameObject movementDisplay)
    {
        GameObject ship_display = Instantiate(ship_sprite_display, transform.position + new Vector3(0, 1, 0), Quaternion.identity);
        color(ship);
        ship_display.GetComponent<SpriteRenderer>().sprite = ship.GetComponent<SpriteRenderer>().sprite;
        ship_display.transform.SetParent(transform);
        GameObject ship_name = Instantiate(text_display_prefab, transform.position + new Vector3(0,(float)-.125,0), Quaternion.identity);
        ship_name.transform.SetParent(transform);
        ship_name.GetComponent<TextMeshPro>().SetText(ship.GetComponent<ShipScript>().name);
        GameObject ship_description = Instantiate(text_display_prefab, transform.position + new Vector3(0, (float)-.75, 0), Quaternion.identity);
        ship_description.transform.SetParent(transform);
        ship_description.GetComponent<TextMeshPro>().SetText(ship.GetComponent<ShipScript>().description);

        SetStats(Instantiate(priceDisplay, transform.position, Quaternion.identity), transform.position + new Vector3((float)-.9,(float)1.9,0));
        SetStats(Instantiate(movementDisplay, transform.position, Quaternion.identity), transform.position + new Vector3((float)-.9, (float)-1.9, 0));
        //SetStats(Instantiate(cargoDisplay, transform.position, Quaternion.identity), transform.position + new Vector3((float).9, (float)-1.9, 0));


    }

    public void SetStats(GameObject display, Vector3 position)
    {
        display.transform.SetParent(transform);
        display.GetComponent<SpriteRenderer>().sortingLayerName = "Card_Ship";
        display.GetComponent<StatHolder>().text_display.GetComponent<MeshRenderer>().sortingLayerName = "Card_Text";
        display.transform.position = position;
    }

    public void color(GameObject ship)
    {
        ship.GetComponent<SpriteRenderer>().sprite = GetComponentInParent<BoardScript>().get_player_number() == 0 ? ship.GetComponent<ShipScript>().big_red : ship.GetComponent<ShipScript>().big_blue;
    }
     

    public void OnMouseExit()
    {
        Destroy(gameObject);
    }
}