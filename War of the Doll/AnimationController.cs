using System.Collections;
using System.Collections.Generic;
using UnityEngine;


public class AnimationController : MonoBehaviour {
    Animation anim;
    public int weaponIndex; //무기 종류 저장

    [System.Serializable]
    public class AnimationSet //무기에 따른 애니메이션을 우선순위별로 저장한 클래스
    {
        public List<AnimationClip> standardAni; //가만히 있기, 움직이기(우선순위 0(가장 낮음))
        public List<AnimationClip> atkAni; //공격(우선순위 1)
        public List<AnimationClip> damageAni; //공격당함(우선순위 2)
        public AnimationClip death;//죽음(우선순위 3(가장 높음))
    }
    
    public List<AnimationSet> weapon;

    // Use this for initialization
    void Start () {
        anim = GetComponent<Animation>();//현재 오브젝트의 애니메이션
        anim.wrapMode = WrapMode.Loop; //기본적으로 무한반복
        int weaponLength = weapon.Count;
        for(int i =0; i< weaponLength; i++) //무기의 종류에 따른 반복문
        {
            int atkLength = weapon[i].atkAni.Count;
            for(int j=0;j<atkLength;j++) //공격 애니메이션의 종류 반복문, 우선순위 설정을 위함
            {
                anim[weapon[i].atkAni[j].name].wrapMode = WrapMode.Once;
                anim[weapon[i].atkAni[j].name].layer = 1;
            }
            int dmgLength = weapon[i].damageAni.Count;
            for (int j = 0; j < dmgLength; j++)//공격 받는 애니메이션의 종류 반복문, 우선순위 설정을 위함
            {
                anim[weapon[i].damageAni[j].name].wrapMode = WrapMode.Once;
                anim[weapon[i].damageAni[j].name].layer = 2;
            }
            anim[weapon[i].death.name].wrapMode = WrapMode.Once; //죽는 애니메이션
            anim[weapon[i].death.name].layer = 3;
        }
        anim.Stop();
    }

    public string GetAniName(string _anim) //무기 종류에 따라 애니메이션을 다르게 불러오기 위한 함수
    {
        if (weaponIndex == (int)eWEAPON.em_SWORDANDSHIELD) //칼, 방패 애니메이션
        {
            if (_anim == "Idle")
                return weapon[(int)eWEAPON.em_SWORDANDSHIELD].standardAni[(int)eANIMATION.em_IDLE].name;
            else if (_anim == "Move")
                return weapon[(int)eWEAPON.em_SWORDANDSHIELD].standardAni[(int)eANIMATION.em_MOVE].name;
            else if (_anim == "Attack01")
                return weapon[(int)eWEAPON.em_SWORDANDSHIELD].atkAni[(int)eANIMATION.em_ATTACK01].name;
            else if (_anim == "Attack02")
                return weapon[(int)eWEAPON.em_SWORDANDSHIELD].atkAni[(int)eANIMATION.em_ATTACK02].name;
            else if (_anim == "Critical01")
                return weapon[(int)eWEAPON.em_SWORDANDSHIELD].atkAni[(int)eANIMATION.em_ATTACK03].name;
            else if (_anim == "Critical02")
                return weapon[(int)eWEAPON.em_SWORDANDSHIELD].atkAni[(int)eANIMATION.em_ATTACK04].name;
            else if (_anim == "GetDamage01")
                return weapon[(int)eWEAPON.em_SWORDANDSHIELD].damageAni[(int)eANIMATION.em_DAMEGE01].name;
            else if (_anim == "GetDamage02")
                return weapon[(int)eWEAPON.em_SWORDANDSHIELD].damageAni[(int)eANIMATION.em_DAMEGE02].name;
            else if (_anim == "Death")
                return weapon[(int)eWEAPON.em_SWORDANDSHIELD].death.name;

        }
        else if (weaponIndex == (int)eWEAPON.em_GREATESWORD || weaponIndex==(int)eWEAPON.em_STICK) //대검, 막대기 애니메이션
        {
            if (_anim == "Idle")
                return(weapon[(int)eWEAPON.em_GREATESWORD].standardAni[(int)eANIMATION.em_IDLE].name);
            else if (_anim == "Move")
                return (weapon[(int)eWEAPON.em_GREATESWORD].standardAni[(int)eANIMATION.em_MOVE].name);
            else if (_anim == "Attack01")
                return (weapon[(int)eWEAPON.em_GREATESWORD].atkAni[(int)eANIMATION.em_ATTACK01].name);
            else if (_anim == "Attack02")
                return(weapon[(int)eWEAPON.em_GREATESWORD].atkAni[(int)eANIMATION.em_ATTACK02].name);
            else if (_anim == "Critical01")
                return (weapon[(int)eWEAPON.em_GREATESWORD].atkAni[(int)eANIMATION.em_ATTACK03].name);
            else if (_anim == "Critical02")
                return (weapon[(int)eWEAPON.em_GREATESWORD].atkAni[(int)eANIMATION.em_ATTACK04].name);
            else if (_anim == "GetDamage01")
                return (weapon[(int)eWEAPON.em_GREATESWORD].damageAni[(int)eANIMATION.em_DAMEGE01].name);
            else if (_anim == "GetDamage02")
                return (weapon[(int)eWEAPON.em_GREATESWORD].damageAni[(int)eANIMATION.em_DAMEGE02].name);
            else if (_anim == "Death")
                return (weapon[(int)eWEAPON.em_GREATESWORD].death.name);
        }
        else if (weaponIndex == (int)eWEAPON.em_DAGGER) //단검 애니메이션
        {
            if (_anim == "Idle")
                return (weapon[(int)eWEAPON.em_DAGGER].standardAni[(int)eANIMATION.em_IDLE].name);
            else if (_anim == "Move")
                return (weapon[(int)eWEAPON.em_DAGGER].standardAni[(int)eANIMATION.em_MOVE].name);
            else if (_anim == "Attack01")
                return (weapon[(int)eWEAPON.em_DAGGER].atkAni[(int)eANIMATION.em_ATTACK01].name);
            else if (_anim == "Attack02")
                return (weapon[(int)eWEAPON.em_DAGGER].atkAni[(int)eANIMATION.em_ATTACK02].name);
            else if (_anim == "Critical01")
                return (weapon[(int)eWEAPON.em_DAGGER].atkAni[(int)eANIMATION.em_ATTACK03].name);
            else if (_anim == "Critical02")
                return (weapon[(int)eWEAPON.em_DAGGER].atkAni[(int)eANIMATION.em_ATTACK04].name);
            else if (_anim == "GetDamage01")
                return (weapon[(int)eWEAPON.em_DAGGER].damageAni[(int)eANIMATION.em_DAMEGE01].name);
            else if (_anim == "GetDamage02")
                return (weapon[(int)eWEAPON.em_DAGGER].damageAni[(int)eANIMATION.em_DAMEGE02].name);
            else if (_anim == "Death")
                return (weapon[(int)eWEAPON.em_DAGGER].death.name);
        }
        else if (weaponIndex == (int)eWEAPON.em_BOW) //활 애니메이션
        {
            if (_anim == "Idle")
                return (weapon[(int)eWEAPON.em_BOW].standardAni[(int)eANIMATION.em_IDLE].name);
            else if (_anim == "Move")
                return (weapon[(int)eWEAPON.em_BOW].standardAni[(int)eANIMATION.em_MOVE].name);
            else if (_anim == "Attack01")
                return (weapon[(int)eWEAPON.em_BOW].atkAni[(int)eANIMATION.em_ATTACK01].name);
            else if (_anim == "Attack02")
                return (weapon[(int)eWEAPON.em_BOW].atkAni[(int)eANIMATION.em_ATTACK02].name);
            else if (_anim == "Critical01")
                return (weapon[(int)eWEAPON.em_BOW].atkAni[(int)eANIMATION.em_ATTACK03].name);
            else if (_anim == "Critical02")
                return (weapon[(int)eWEAPON.em_BOW].atkAni[(int)eANIMATION.em_ATTACK04].name);
            else if (_anim == "GetDamage01")
                return (weapon[(int)eWEAPON.em_BOW].damageAni[(int)eANIMATION.em_DAMEGE01].name);
            else if (_anim == "GetDamage02")
                return (weapon[(int)eWEAPON.em_BOW].damageAni[(int)eANIMATION.em_DAMEGE02].name);
            else if (_anim == "Death")
                return (weapon[(int)eWEAPON.em_BOW].death.name);
        }
        else if (weaponIndex == (int)eWEAPON.em_WAND) //마법봉 애니메이션
        {
            if (_anim == "Idle")
                return (weapon[(int)eWEAPON.em_WAND].standardAni[(int)eANIMATION.em_IDLE].name);
            else if (_anim == "Move")
                return (weapon[(int)eWEAPON.em_WAND].standardAni[(int)eANIMATION.em_MOVE].name);
            else if (_anim == "Attack01")
                return (weapon[(int)eWEAPON.em_WAND].atkAni[(int)eANIMATION.em_ATTACK01].name);
            else if (_anim == "Attack02")
                return (weapon[(int)eWEAPON.em_WAND].atkAni[(int)eANIMATION.em_ATTACK02].name);
            else if (_anim == "Critical01")
                return (weapon[(int)eWEAPON.em_WAND].atkAni[(int)eANIMATION.em_ATTACK03].name);
            else if (_anim == "Critical02")
                return (weapon[(int)eWEAPON.em_WAND].atkAni[(int)eANIMATION.em_ATTACK04].name);
            else if (_anim == "GetDamage01")
                return (weapon[(int)eWEAPON.em_WAND].damageAni[(int)eANIMATION.em_DAMEGE01].name);
            else if (_anim == "GetDamage02")
                return (weapon[(int)eWEAPON.em_WAND].damageAni[(int)eANIMATION.em_DAMEGE02].name);
            else if (_anim == "Death")
                return (weapon[(int)eWEAPON.em_WAND].death.name);
        }
        return "Fail";
    }

    public void PlayAnimation(string aniName) //걷기, idle애니메이션 재생
    {
        anim.CrossFade(GetAniName(aniName));
    }

    public void PlayAtkDmg(string aniName) //공격, 데미지 애니메이션 재생
    {
        anim.wrapMode = WrapMode.Once;
        anim.Play(GetAniName(aniName));
    }

    public void PlayDeath(string aniName) //죽는 애니메이션
    {
        anim.wrapMode = WrapMode.ClampForever;
        anim.Play(GetAniName(aniName));
    }

    public float GetAniLength(string aniName) //애니메이션 재생 시간 가져오기
    {
        return anim[GetAniName(aniName)].length;
    }
}

