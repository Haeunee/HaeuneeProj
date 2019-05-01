using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using UnityEngine.SceneManagement;

public class HeroCustomize : MonoBehaviour {

	[System.Serializable]
	public class IndexModel //옷, 외형 관련 인덱스
	{
		public int CurrentIndex,MaxIndex;
	}

	[System.Serializable]
	public class IndexModelWeapon //무기 관련 인덱스
	{
		public int CurrentIndex,CurrentType,MaxIndexType,MaxIndexWeapon;
	}

	public string Gender; //성별

	public IndexModel IndexHair,IndexColorHair,IndexFace,IndexSuit;
	public IndexModelWeapon IndexWeapon;

	public List<GameObject> HairMale,HairFemale; //헤어 리스트
	public Material Hair_Material; //머리 색 변경시킬 메테리얼
	public List<Color> HairColor; //머리색 리스트
	public GameObject FaceMale_OBJ,FaceFemale_OBJ; //얼굴 오브젝트
	public List<Texture2D> FaceMale,FaceFemale; //얼굴 텍스쳐
	public List<GameObject> SuitMale,SuitFemale; //옷
	public List<GameObject> Stick,Sword,Shield,GreatSword,DaggerL,DaggerR,Bow,Wand; //무기

	void Start()
    {
        UpdateVisual();
        UpdateWeapon();
    }

	public void HideAllObject(List<GameObject> _List) //숨기기
	{
		for(int i=0;i<_List.Count;i++)
		{
			_List[i].SetActive(false);
		}
	}

	public void ShowIndex(List<GameObject> _List,int _Index) //하나 보여주기
	{
		for(int i=0;i<_List.Count;i++)
		{
			if(i == _Index)
			{
				_List[i].SetActive(true);
				return;
			}

		}
	}

	public void SetHairColor(List<Color> _List,int _Index) //헤어 컬러 변경
	{
		for(int i=0;i<_List.Count;i++)
		{
			if(i == _Index)
			{
				Hair_Material.color = _List[i];
				return;
			}
			
		}
	}

	public void SetTextureToFace(List<Texture2D> _List,int _Index) //얼굴 텍스쳐 변경
	{
		for(int i=0;i<_List.Count;i++)
		{
			if(i == _Index)
			{
				if(Gender == "Male")
				{
					FaceMale_OBJ.GetComponent<SkinnedMeshRenderer>().material.mainTexture = _List[i];
				}else if(Gender == "Female")
				{
					FaceFemale_OBJ.GetComponent<SkinnedMeshRenderer>().material.mainTexture = _List[i];
				}

				return;
			}
			
		}
	}

    public void HideWeapon() //무기 숨기기
    {
        HideAllObject(Sword);
        HideAllObject(Shield);
        HideAllObject(GreatSword);
        HideAllObject(DaggerL);
        HideAllObject(DaggerR);
        HideAllObject(Bow);
        HideAllObject(Wand);
        HideAllObject(Stick);
    }

    public void HideVisual() //외형, 옷 숨기기
    {
        HideAllObject(HairMale);
        HideAllObject(SuitMale);
        FaceMale_OBJ.SetActive(false);
        HideAllObject(HairFemale);
        HideAllObject(SuitFemale);
        FaceFemale_OBJ.SetActive(false);
    }

    public void UpdateVisual() //외형, 옷 보이게 하기
    {
        HideVisual();
        //Show select obj
        if (Gender == "Male")
        {
            ShowIndex(HairMale, IndexHair.CurrentIndex);
            ShowIndex(SuitMale, IndexSuit.CurrentIndex);
            SetTextureToFace(FaceMale, IndexFace.CurrentIndex);
            FaceMale_OBJ.SetActive(true);
        }
        else if (Gender == "Female")
        {
            ShowIndex(HairFemale, IndexHair.CurrentIndex);
            ShowIndex(SuitFemale, IndexSuit.CurrentIndex);
            SetTextureToFace(FaceFemale, IndexFace.CurrentIndex);
            FaceFemale_OBJ.SetActive(true);
        }

        //Set Hair Color
        SetHairColor(HairColor, IndexColorHair.CurrentIndex);
    }

    public void UpdateWeapon() //무기 보이게 하기
    {
        HideWeapon();
        if (IndexWeapon.CurrentType == 0)
        {
            ShowIndex(Sword, IndexWeapon.CurrentIndex);
            ShowIndex(Shield, IndexWeapon.CurrentIndex);
        }
        else if (IndexWeapon.CurrentType == 1)
        {
            ShowIndex(GreatSword, IndexWeapon.CurrentIndex);
        }
        else if (IndexWeapon.CurrentType == 2)
        {
            ShowIndex(DaggerL, IndexWeapon.CurrentIndex);
            ShowIndex(DaggerR, IndexWeapon.CurrentIndex);
        }
        else if (IndexWeapon.CurrentType == 3)
        {
            ShowIndex(Bow, IndexWeapon.CurrentIndex);
        }
        else if (IndexWeapon.CurrentType == 4)
        {
            ShowIndex(Wand, IndexWeapon.CurrentIndex);
        }
        else if(IndexWeapon.CurrentType == -1)
        {
            ShowIndex(Stick, 0);
        }
    }
}
