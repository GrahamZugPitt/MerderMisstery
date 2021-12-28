using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TileTracker : MonoBehaviour
{
    public int last_object_position;
    public int sprite_count;
    GameObject[] sprites_on_stack = new GameObject[20];
    public GameObject trackingCubePrefab;

    public void add_sprite(Sprite sprite)
    {
        GameObject sprite_holder = Instantiate(trackingCubePrefab, transform.position + new Vector3(sprite_count, 0, 0), Quaternion.identity);
        sprite_holder.GetComponent<SpriteRenderer>().sprite = sprite;
        sprites_on_stack[sprite_count++] = sprite_holder;
    }
    public void unload()
    {
        for (int i = 0; i < sprite_count; i++)
        {
            Destroy(sprites_on_stack[i]);
        }
            sprite_count = 0;
    }
}
