using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CustomBtn : MonoBehaviour {
    HeroCustomize heroCustomize;

    void Start () {
        heroCustomize = GetComponent<HeroCustomize>();
        StartCoroutine(HideWeapon());
    }
	
    public void MaleBtn() //남자로 바꾸기
    {
        heroCustomize.Gender = "Male";
        heroCustomize.UpdateVisual();
    }

    public void FemaleBtn() //여자로 바꾸기
    {
        heroCustomize.Gender = "Female";
        heroCustomize.UpdateVisual();
    }

    public void HairBtn() //머리 변경
    {
        heroCustomize.IndexHair.CurrentIndex++;

        if (heroCustomize.IndexHair.CurrentIndex > heroCustomize.IndexHair.MaxIndex)
            heroCustomize.IndexHair.CurrentIndex = 0;

        heroCustomize.UpdateVisual();
    }

    public void HairColorBtn() //머리색 변경
    {
        heroCustomize.IndexColorHair.CurrentIndex++;

        if (heroCustomize.IndexColorHair.CurrentIndex > heroCustomize.IndexColorHair.MaxIndex)
            heroCustomize.IndexColorHair.CurrentIndex = 0;

        heroCustomize.UpdateVisual();
    }

    public void FaceBtn() //얼굴 변경
    {
        heroCustomize.IndexFace.CurrentIndex++;

        if (heroCustomize.IndexFace.CurrentIndex > heroCustomize.IndexFace.MaxIndex)
            heroCustomize.IndexFace.CurrentIndex = 0;

        heroCustomize.UpdateVisual();
    }

    IEnumerator HideWeapon() //무기 숨기기
    {
        yield return new WaitForSeconds(0.01f);
        heroCustomize.HideWeapon();
    }
}
