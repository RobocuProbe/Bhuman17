/** A test striker option without common decision */
option(Striker_Backup)
{
  initial_state(start)
  {
    transition
    {
      if(state_time > 1000)
        goto turnToBall;
    }
    action
    {
      theHeadControlMode = HeadControl::lookForward;
      Stand();
    }
  }

  state(turnToBall)
  {
    transition
    {
      if(theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
      {
          
              goto searchForBall;
      }
        
      if(std::abs(theBallModel.estimate.position.angle()) < 5_deg)
      {
          
              goto walkToBall;
      }
        
    }
    action
    {
      theHeadControlMode = HeadControl::lookForward;
      WalkToTarget(Pose2f(50.f, 50.f, 50.f), Pose2f(theBallModel.estimate.position.angle(), 0.f, 0.f));
    }
  }

  state(walkToBall)
  {
    transition
    {
      if(theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
      {
              goto searchForBall;
      }
        
      if(theBallModel.estimate.position.norm() < 500.f)
      {
          
              goto alignBehindBall;
      }
    }
    action
    {
      theHeadControlMode = HeadControl::lookForward;
      //theMotionRequest=thePathPlanner.plan(theBallModel.estimate.position, Pose2f(50.f, 50.f, 50.f), false);
      WalkToTarget(Pose2f(50.f, 50.f, 50.f), theBallModel.estimate.position);
    }
  }

  state(alignToGoal)
  {
    transition
    {
      if(theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
      {
          
              goto searchForBall;
      }
        
      if(std::abs(theLibCodeRelease.angleToGoal) < 10_deg && std::abs(theBallModel.estimate.position.y()) < 100.f)
      {
              goto alignBehindBall;
      }
    }
    action
    {
      theHeadControlMode = HeadControl::lookForward;
      WalkToTarget(Pose2f(100.f, 100.f, 100.f), Pose2f(theLibCodeRelease.angleToGoal, theBallModel.estimate.position.x() - 400.f, theBallModel.estimate.position.y()));
    }
  }

  state(alignBehindBall)
  {
    transition
    {
        OUTPUT_TEXT("alignBehindBall");
          if(theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
      {
              goto searchForBall;
      }
      if(theLibCodeRelease.between(theBallModel.estimate.position.y(), -45.f, -25.f)
      && theLibCodeRelease.between(theBallModel.estimate.position.x(), 20.f, 200.f))
      //&& std::abs(theLibCodeRelease.angleToGoal) < 2_deg)
      {
          
              goto kick;
      }
        
    }
    action
    {
      theHeadControlMode = HeadControl::lookForward;
      WalkToTarget(Pose2f(80.f, 30.f, 30.f), Pose2f(0.f, theBallModel.estimate.position.x() - 100.f, theBallModel.estimate.position.y() + 38.f));
    }
  }

  state(kick)
  {
    transition
    {
      if(state_time > 3000 || (state_time > 10 && action_done))
      {
          
              goto start;
      }
    }
    action
    {
      theHeadControlMode = HeadControl::lookForward;
      InWalkKick(WalkKickVariant(WalkKicks::forward, Legs::left), 
                                    Pose2f(theLibCodeRelease.angleToGoal, theBallModel.estimate.position.x() - 160.f, theBallModel.estimate.position.y() - 55.f));
    }
  }
  
    state(shoot)
  {
    transition
    {
        OUTPUT_TEXT("shoot");
        //OUTPUT_TEXT("Striker_shoot");
        if(theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
          {
                  goto searchForBall;
          }
      if(state_time > 3000 || (state_time > 10 && action_done))
      {
          goto start;
      }
    }
    action
    {
      //OUTPUT_TEXT("Striker_kick_action");
     FollowTheTarget(theBallModel.estimate.position.angle(), theLibCodeRelease.tilt);
      Shoot(KickRequest::greatKickForward);
    }
  }

  state(searchForBall)
  {
    transition
    {
      if(theLibCodeRelease.timeSinceBallWasSeen < 300)
      {
          
              goto turnToBall;
      }
    }
    action
    {
      theHeadControlMode = HeadControl::lookForward;
      WalkAtRelativeSpeed(Pose2f(0.5, 0.f, 0.f));
    }
  }
}
