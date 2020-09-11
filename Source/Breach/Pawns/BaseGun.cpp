// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseGun.h"
#include "Breach/Characters/PlayerCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"

#define OUT

// Sets default values
ABaseGun::ABaseGun()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
}

// Called when the game starts or when spawned
void ABaseGun::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABaseGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseGun::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABaseGun::PullTrigger()
{
	if (MuzzleFlash != nullptr)
	{
		UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, TEXT("Muzzle"));
	}
	FHitResult ShotResult;
	FVector ShotDirection;
	bool bHitSuccess = GunTrace(OUT ShotResult, OUT ShotDirection);
	if (bHitSuccess)
	{
		AActor* HitActor = ShotResult.GetActor();
		if (HitActor != nullptr)
		{
			FPointDamageEvent GunDamageEvent(GunDamage, ShotResult, ShotDirection, nullptr);
			HitActor->TakeDamage(GunDamage, GunDamageEvent, GetOwnerController(), this);
		}
		if (ImpactFlash != nullptr)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactFlash, ShotResult.Location, ShotDirection.Rotation(), true);
		}
	}
}
	
AController* ABaseGun::GetOwnerController() const
{
	APlayerCharacter* OwnerPawn = Cast< APlayerCharacter>(GetOwner());
	if (OwnerPawn == nullptr) return nullptr;
	AController* OwnerController = OwnerPawn->GetController();

	return OwnerController;
}

bool ABaseGun::GunTrace(FHitResult& ShotResult, FVector& ShotDirection)
{
	if (GetOwnerController() == nullptr) return false;

	FVector OwnerLocation;
	FRotator OwnerRotation;
	GetOwnerController()->GetPlayerViewPoint(OUT OwnerLocation, OUT OwnerRotation);
	ShotDirection = -OwnerRotation.Vector();
	FVector LineEnd = OwnerLocation + OwnerRotation.Vector() * MaxRange;
	ShotResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());

	return GetWorld()->LineTraceSingleByChannel(OUT ShotResult, OwnerLocation, LineEnd, ECollisionChannel::ECC_GameTraceChannel1, Params);
}

int32 ABaseGun::GetMaxBullets()
{
	return MaxBullets;
}

float ABaseGun::GetFireRate()
{
	return FireRate;
}