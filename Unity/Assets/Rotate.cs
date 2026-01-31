using UnityEngine;

public class Rotate : MonoBehaviour
{

    public UDPReceiver udpReceiver; // Reference to the UDPReceiver script

    public Material mat;


    // Start is called once before the first execution of Update after the MonoBehaviour is created
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        float tempval = Mathf.Round((udpReceiver.potentiometer)/1024f*100f);
        float laser = udpReceiver.distance;

        if (laser < 30f)
        {
            tempval = 0f;
        }
        mat.SetFloat("_Burette_Fluid", tempval/100f);
        gameObject.transform.rotation = Quaternion.Euler(0, tempval, 0);
    }
}
