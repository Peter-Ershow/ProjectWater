// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectWater/Actors/CPP_Screenfade.h"

// Sets default values
ACPP_Screenfade::ACPP_Screenfade()
{
	PrimaryActorTick.bCanEverTick = true;
	
	Sphere = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SphereMeshComponent"));
	
	RootComponent = Sphere;
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
    
	if (SphereMesh.Succeeded())
	{
		Sphere->SetStaticMesh(SphereMesh.Object);
	}
	
	Sphere->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	Sphere->SetMobility(EComponentMobility::Movable);
}

// Called when the game starts or when spawned
void ACPP_Screenfade::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACPP_Screenfade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

