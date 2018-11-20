using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ComputerClick : MonoBehaviour {

    private GameObject m_cam;
    private GameObject m_laptop;

    void Awake()
    {
        m_cam = GameObject.FindWithTag("MainCamera");
        Debug.Assert(m_cam != null);
        m_laptop = GameObject.Find("laptop");
        Debug.Assert(m_laptop != null);
    }

	// Use this for initialization
	void Start () {
	}

    void OnTriggerStay(Collider coll)
    {
        if(coll.gameObject.tag == "Player" && Input.GetKeyDown(KeyCode.Space))
        {
            var camfollowscript = m_cam.GetComponent<CameraFollow>();
            if(camfollowscript.target == coll.gameObject) 
                camfollowscript.changeTarget(m_laptop);
            else if(camfollowscript.target == m_laptop)
                camfollowscript.changeTarget(coll.gameObject);
        }
    }
}
