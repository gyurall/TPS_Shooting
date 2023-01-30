// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSProject.h"
#include "TPSPlayer.h"
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include "PlayerAnim.h"
#include "PlayerMove.h"
#include "PlayerFire.h"
#include <Kismet/GameplayStatics.h>




ATPSPlayer::ATPSPlayer()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	playerMove = CreateDefaultSubobject<UPlayerMove>(TEXT("PlayerMove"));

	//playerFire = CreateDefaultSubobject<UPlayerFire>(TEXT("PlayerFire"));


	// 1. ���̷�Ż�޽� �����͸� �ҷ����� �ʹ�.
	ConstructorHelpers::FObjectFinder<USkeletalMesh>
		TempMesh(TEXT("SkeletalMesh'/Game/AnimStarterPack/UE4_Mannequin/Mesh/SK_Mannequin.SK_Mannequin'"));
	if (TempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(TempMesh.Object);
		// 2. Mesh ������Ʈ�� ��ġ�� ȸ�� ���� �����ϰ� �ʹ�.
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}

	// 3. TPS ī�޶� ���̰� �ʹ�.
	// 3-1. SpringArm ������Ʈ ���̱�
	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	springArmComp->SetupAttachment(RootComponent);
	springArmComp->SetRelativeLocation(FVector(0, 70, 90));
	springArmComp->TargetArmLength = 400;
	springArmComp->bUsePawnControlRotation = true;
	// 3-2. Camera ������Ʈ ���̱�
	tpsCamComp = CreateDefaultSubobject<UCameraComponent>(TEXT("TpsCamComp"));
	tpsCamComp->SetupAttachment(springArmComp);
	tpsCamComp->bUsePawnControlRotation = false;

	bUseControllerRotationYaw = true;
	// 2�� ����
	JumpMaxCount = 2;

	// 4. �� ���̷�Ż�޽� ������Ʈ ���
	gunMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMeshComp"));
	// 4-1. �θ� ������Ʈ�� Mesh ������Ʈ�� ����
	gunMeshComp->SetupAttachment(GetMesh(), TEXT("hand_rSocket"));
	// 4-2. ���̷�Ż�޽� ������ �ε�
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempGunMesh(TEXT("SkeletalMesh'/Game/FPWeapon/Mesh/SK_FPGun.SK_FPGun'"));
	// 4-3. �����ͷε尡 �����ߴٸ�
	if (TempGunMesh.Succeeded())
	{
		// 4-4. ���̷�Ż�޽� ������ �Ҵ�
		gunMeshComp->SetSkeletalMesh(TempGunMesh.Object);
		// 4-5. ��ġ �����ϱ�
		gunMeshComp->SetRelativeLocation(FVector(-17, 10, -3));
		gunMeshComp->SetRelativeRotation(FRotator(0, 90, 0));
	}

	// 5. �������۰� ������Ʈ ���
	sniperGunComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SniperGunComp"));
	// 5-1. �θ� ������Ʈ�� Mesh ������Ʈ�� ����
	sniperGunComp->SetupAttachment(GetMesh(), TEXT("hand_rSocket"));
	// 5-2. ����ƽ�޽� ������ �ε�
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempSniperMesh(TEXT("StaticMesh'/Game/SniperGun/sniper1.sniper1'"));
	// 5-3. �����ͷε尡 �����ߴٸ�
	if (TempSniperMesh.Succeeded())
	{
		// 5-4. ����ƽ�޽� ������ �Ҵ�
		sniperGunComp->SetStaticMesh(TempSniperMesh.Object);
		// 5-5. ��ġ �����ϱ�
		sniperGunComp->SetRelativeLocation(FVector(-42, 7, 1));
		sniperGunComp->SetRelativeRotation(FRotator(0, 90, 0));
		// 5-6. ũ�� �����ϱ�
		sniperGunComp->SetRelativeScale3D(FVector(0.15f));
	}



}

// Called when the game starts or when spawned
void ATPSPlayer::BeginPlay()
{
	Super::BeginPlay();

	hp = initialHp;

	GetMesh()->SetSimulatePhysics(false);

	isDead = false;
}

// Called every frame
void ATPSPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATPSPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// ������Ʈ���� �Է� ���ε� ó���ϵ��� ȣ��

	onInputBindingDelegate.Broadcast(PlayerInputComponent);

	/*playerMove->SetupInputBinding(PlayerInputComponent);

	playerFire->SetupInputBinding(PlayerInputComponent);*/



}

void ATPSPlayer::OnHitEvent()
{

	// �ǰ� �ִϸ��̼� ���
	//auto anim = Cast<UPlayerAnim>(GetMesh()->GetAnimInstance());
	//anim->PlayDamageAnim();

	PRINT_LOG(TEXT("Damaged !!!"));
	hp--;

	if (hp > 0)
	{
		OnDamaged();
	}

	else if (hp <= 0)
	{
		PRINT_LOG(TEXT("Player is dead!"));

		DoRagdoll();

		OnGameOver();	

		isDead = true;
	}
}

void ATPSPlayer::DoRagdoll()
{
	if (isDead == false)
	{
		GetMesh()->SetSimulatePhysics(true);

		PRINT_LOG(TEXT("Ragdoll!!!"));
	}

}


void ATPSPlayer::OnGameOver_Implementation()
{
	if (isDead == false)
	{
		FTimerHandle WaitHandle;
		float WaitTime = 3.0f; //�ð��� �����ϰ�
		GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
			{

				// ���ӿ����� �Ͻ�����
				UGameplayStatics::SetGamePaused(GetWorld(), true);
				PRINT_LOG(TEXT("GamePause!!!"));
			}), WaitTime, false); //�ݺ��� ���⼭ �߰� ������ ������ ��������	
	}

}
