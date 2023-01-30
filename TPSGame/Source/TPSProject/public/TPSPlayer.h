// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerAnim.h"
#include "TPSPlayer.generated.h"



DECLARE_MULTICAST_DELEGATE_OneParam(FInputbindingDelegate, class UInputComponent*);


UCLASS()
class TPSPROJECT_API ATPSPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// �Է¹��ε� ��������Ʈ
	FInputbindingDelegate onInputBindingDelegate;




public:
	// Sets default values for this character's properties
	ATPSPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;



public:
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class USpringArmComponent* springArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class UCameraComponent* tpsCamComp;

	
	

	// �� ���̷�Ż�޽�
	UPROPERTY(VisibleAnywhere, Category = GunMesh)
	class USkeletalMeshComponent* gunMeshComp;

	

	// �������۰� ����ƽ�޽� �߰�
	UPROPERTY(VisibleAnywhere, Category = GunMesh)
	class UStaticMeshComponent* sniperGunComp;

public:

	UPROPERTY(VisibleAnywhere, Category = Component)
	class UPlayerBaseComponent* playerMove;

	UPROPERTY(VisibleAnywhere, Category = Component)
	class UPlayerBaseComponent* playerFire;

	// ��� ���� �ִϸ��̼� ��������Ʈ
	UPROPERTY()
	class UPlayerAnim* anim;


	//���� ü��
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Health)
	int32 hp;
	// �ʱ� hp ��
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Health)
	int32 initialHp = 10;

	// �ǰ� �������� ó��
	UFUNCTION(BlueprintCallable, Category = Health)
	void OnHitEvent();

	// ���� ������ �� ȣ��� �Լ�
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Health)
	void OnGameOver();

	// �� �ٲܶ� ȣ��Ǵ� �̺�Ʈ �Լ�
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = Health)
	void OnUsingGrenade(bool isGrenade);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnDamaged();

	UFUNCTION(BlueprintCallable)
	void DoRagdoll();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool isDead = false;
};