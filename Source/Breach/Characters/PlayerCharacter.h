// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class ABaseGun;
class UAnimMontage;

UCLASS()
class BREACH_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	void EnemyShoot();

	float Health;

	UFUNCTION(BlueprintPure)
		float GetCurrentHealth();
	UFUNCTION(BlueprintPure)
		bool IsDead();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


private:	
	void MoveUp(float AxisValue);
	void MoveSideways(float AxisValue);
	void StartPullTrigger();
	void StopPullTrigger();
	void Shoot();
	void Reload();

	int32 Bullets;
	float FireRate;
	bool bIsFiring = false;
	FTimerHandle FireRateHandle;

	UPROPERTY()
		ABaseGun* Gun;
	UPROPERTY(EditAnywhere, Category = "Weapons")
		TSubclassOf<ABaseGun> EquippedGun; 
	UPROPERTY(EditAnywhere, Category = "Weapons")
		UAnimMontage* FireAnimation;
	UPROPERTY(EditDefaultsOnly, Category = "Weapons")
		USoundBase* ReloadSound;
	UPROPERTY(EditAnywhere, Category = "Health")
		float MaxHealth = 100;
	UPROPERTY(EditAnywhere, Category = "Health")
		bool bIsEnemy = true;
};