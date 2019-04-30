using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class ShotController : MonoBehaviour
{
    public Vector3 rayPoint;
    GameObject MagicEffect;
    GameObject StickEffect;
    EffSoundController effSound;
    Scene scene;
    tutorial tuto;

    private void Awake()
    {
        scene = SceneManager.GetActiveScene();
        if (scene.name == "GameScene")
        {
            MagicEffect = GameObject.Find("MagicEffect");
            StickEffect = GameObject.Find("StickEffect");
        }
        else if (scene.name == "TutorialScene")
            tuto = GameObject.Find("Player").GetComponent<tutorial>();
    }

    void Start()
    {
        effSound = transform.parent.GetChild(1).GetComponent<EffSoundController>();
    }

    private void OnEnable()
    {
        StartCoroutine(PosDelay()); //0.35초 후 이동 
        transform.Rotate(new Vector3(0, -90, 0));
        effSound.PlayEff((int)eEFFSOUND.em_WIND);
    }

    void Update() //플레이어의 레이 방향으로 이동
    {
        transform.parent.Translate(rayPoint * 0.15f);
    }

    private void OnTriggerEnter(Collider other)
    {//물건이나, 적에 닿으면 파괴
        if (other.gameObject.layer == (int)eLAYER.ENEMY || other.tag == "Shootable" || other.tag == "Player")
        {
            transform.parent.transform.position = new Vector3(1000, 1000, 1000);
            GetComponent<ShotController>().enabled = false;
            if (scene.name == "GameScene")
            {
                if (transform.name == "Magic")
                    MagicEffect.transform.position = other.gameObject.transform.position;
                else if (transform.name == "Arrow")
                    StickEffect.transform.position = other.gameObject.transform.position + Vector3.up * 2;
            }
            else if (scene.name == "TutorialScene")
            {
                tuto.hpBar.value -= 1;
            }
        }
    }

    IEnumerator PosDelay()
    {//0.35초 후 보이지 않게 이동
        yield return new WaitForSeconds(0.35f);
        transform.parent.transform.position = new Vector3(1000, 0, 1000);
        GetComponent<ShotController>().enabled = false;
    }
}
