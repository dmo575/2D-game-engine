#pragma once
#include "Component.h"//        Superclass

/*ABOUT:
* This is a UserComp class made for the assignment.
* Calls on 4 animations for moving each direction
* It also calls an IDLE animation but right after that it sets the Animator component to
* disables because the module requests to stop the animation (wouldnt make difference anyways
* since idle is 1 frame in this case)
* 
* Something interesting about this class is that it is the first class where I let a component
* use the Entity to create new components.
* I think thata nice way of using this project is doing just that: Have a master component
* that initializes all other ones plus some optional game logic like in this case (Handling
* cursor input)
*
*/

class EC_Transform;
class EC_Animator;

class UC_Cursor : public Component
{
public:
    UC_Cursor(float vel);
    virtual ~UC_Cursor() override;

    void SetVel(float newVel);

private:
    virtual void Init() override;
    virtual void Update(float delta) override;

private:
    EC_Transform* m_pTransform;
    EC_Animator* m_pAnimator;
    float m_vel;
};