using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class PlayerCameraScript : MonoBehaviour {
    Quaternion initRot; //초기 회전각
    Quaternion preRot; //예측한 회전각  
    public float sensibility = 5f; //회전운동 민감도
    public int MaxRot = 30; //최대 회전각 크기 
    public int MinRot = -30; //최소 회전각 크기 
    public float rotSpeed = 40; //회전 속도 

    private void Awake()
    {
        initRot = transform.localRotation;
        preRot = transform.localRotation;
    }

    private void OnTriggerEnter(Collider other)
    {
        if (other.gameObject.layer == (int)eLAYER.TOUCHWALL) 
            //카메라가 벽에 부딪히면 레이어를 변경시켜서 벽이 안보이게 함
        {
            other.gameObject.layer = (int)eLAYER.WALL;
        }
    }

    private void OnTriggerExit(Collider other)
    {
        if (other.gameObject.layer == (int)eLAYER.WALL)
        {
            other.gameObject.layer = (int)eLAYER.TOUCHWALL;
        }
    }
    
    void LateUpdate ()
    {
        if (Input.GetMouseButton(1)&&SceneManager.GetActiveScene().name== "ItemCollectScene") //카메라 위 아래 회전 
        {
            float RotY = Input.GetAxis("Mouse Y") * sensibility;
            preRot *= Quaternion.Euler(Vector3.left * RotY);
            float angle_Pre2Init = Quaternion.Angle(initRot, preRot);

            if (angle_Pre2Init > MinRot && angle_Pre2Init < MaxRot)
                transform.localRotation = Quaternion.Slerp(transform.localRotation, preRot, rotSpeed * Time.deltaTime);
        }
    }
}
