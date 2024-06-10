// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/BPGameMode.h"

ABPGameMode::ABPGameMode()
{
    static ConstructorHelpers::FClassFinder<APawn> DefaultPawnClassRef(TEXT("/Script/BestPilot.BPPlanePlayer"));
    if (DefaultPawnClassRef.Class)
    {
        DefaultPawnClass = DefaultPawnClassRef.Class;
    }
    
    static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef(TEXT("/Script/BestPilot.BPPlayerController"));
    if (PlayerControllerClassRef.Class)
    {
        PlayerControllerClass = PlayerControllerClassRef.Class;
    }
}
