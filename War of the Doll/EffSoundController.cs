using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class EffSoundController : MonoBehaviour
{
    public AudioSource audioEff;
    Slider effVolSlider;
    public AudioClip[] effClip;

    private void Awake()
    {
        GameObject soundMgr = GameObject.Find("GameMgr");
        GameObject setWin = soundMgr.transform.Find("SettingWin").gameObject;
        effVolSlider = setWin.transform.Find("EffSoundSlider").GetComponent<Slider>();
    }

    public void PlayEff(int effNum)
    { //효과음 재생
        audioEff.clip = effClip[effNum];
        audioEff.volume = effVolSlider.value;
        audioEff.Play();
    }
}
