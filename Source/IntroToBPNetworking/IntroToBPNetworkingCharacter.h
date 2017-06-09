// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "IntroToBPNetworkingCharacter.generated.h"

UCLASS(config=Game)
class AIntroToBPNetworkingCharacter : public ACharacter
{
	GENERATED_BODY()


	// -----------------------------------------------------------------------------------
	//			Tutorial	 Staff
	// -----------------------------------------------------------------------------------
	
public:



protected:

	virtual void BeginPlay() override;
	
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
	/** Simple take damage reducing  */
	void OnTakeDamage(float Damage);

	/** Initialize Character attributes only on the server  */
	UFUNCTION(BlueprintAuthorityOnly, BlueprintCallable, Category = "AAA")
	void InitAttributes();

	/** Runtime visual representation of character's stuff  */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	void UpdateCharacterText();

	/** Returns has player any bombs or not   */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	bool HasBombs() const { return BombCount > 0; }
	
	/** Firing preparation  */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	void Fire();

	/** Reload weapon after successed fire  */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	void Reloading();

	/** Server Fire function  */
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerAttemptSpawnBomb();
	bool ServerAttemptSpawnBomb_Validate() { return true; }
	void ServerAttemptSpawnBomb_Implementation();

	// -----------------------------------------------------------------------------------
	
	/** Character's current health  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	float MaxHealth = 100.f;

	/** Character's  MaxBombCount */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	int32 MaxBombCount = 10;

	/** Character's runtime Help Text */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	class UTextRenderComponent* HelpText;
	
	/** Returns can character fire now or not   */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	uint32 bCanFire : 1;

	/**  Reloading Timer started after success fire shot */
	UPROPERTY()
	FTimerHandle ReloadingTimer;

	/**  Weapon Reloading Delay */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	float ReloadingDelay = 1.f;

	/** Spawn Bomb Location representation */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	class UArrowComponent* SpawnBombLocation;

	/** Bomb Type to spawn  */
 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAA")
 	TSubclassOf<class ABomb> BombType;

	// -----------------------------------------------------------------------------------
	// Rep Notify

	/** Character's health  */
	UPROPERTY(ReplicatedUsing = OnRep_Health, VisibleAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	float Health;
	
	/** Calls every time when Health variable changed */
	UFUNCTION()
	virtual void OnRep_Health();

	/** Current bomb amount */
	UPROPERTY(ReplicatedUsing = OnRep_BombCount, VisibleAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	int32 BombCount;

	/** Calls every time when BombCount variable changed  */
	UFUNCTION()
	virtual void OnRep_BombCount();

	// Rep Notify
	// -----------------------------------------------------------------------------------


private:
	
	// -----------------------------------------------------------------------------------
	//			Default		Character 
	// -----------------------------------------------------------------------------------

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

public:

	AIntroToBPNetworkingCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

protected:

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

protected:

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

