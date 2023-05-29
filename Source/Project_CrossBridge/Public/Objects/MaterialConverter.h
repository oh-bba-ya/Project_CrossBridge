// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "MaterialConverter.generated.h"


DECLARE_DELEGATE(FMAKINGDELEGATE);
DECLARE_DELEGATE(FUNMAKINGDELEGATE);

UCLASS()
class PROJECT_CROSSBRIDGE_API AMaterialConverter : public AActor
{
	GENERATED_BODY()
	
public:

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	// Sets default values for this actor's properties
	AMaterialConverter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, Category="Settings|Property")
	class UBoxComponent* BoxComponent;

	UPROPERTY(EditDefaultsOnly, Category="Settings|Property")
	class UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditDefaultsOnly, Category="Settings|Property")
	class UArrowComponent* Arrow;

	UPROPERTY(EditDefaultsOnly, Category="Settings|Property")
	class UNiagaraComponent* MakingEffectComp;
	
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void MakingHoming();

	UFUNCTION()
	void SetGarbageCount(float v);

	UFUNCTION()
	float GetGarbageCount();

	UFUNCTION()
	void SaveGarbage(float v);


protected:
	UFUNCTION(Server, Reliable)
	void Server_MakingHoming();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_MakingHoming();
	
	UFUNCTION()
	void Entrance(class ABaseCharacter* p);
	
	UFUNCTION(NetMulticast, Unreliable)
	void MultiCast_Entrance(class ABaseCharacter* p);

	UFUNCTION(Server, Unreliable)
	void Server_Entrance(class ABaseCharacter* p);


	UFUNCTION()
	void Exit(class ABaseCharacter* p);

	UFUNCTION(NetMulticast, Unreliable)
	void MultiCast_Exit(class ABaseCharacter* p);

	UFUNCTION(Server, Unreliable)
	void Server_Exit(class ABaseCharacter* p);

	UFUNCTION(Server, Reliable)
	void Server_SaveGarbage(float v);

	

private:
	UPROPERTY(EditDefaultsOnly, Category="Settings|Property")
	TSubclassOf<class AHomingItem> HomingItemFactory;


	UPROPERTY(EditDefaultsOnly, Category="Settings|Property")
	float RequireCount = 2.0f;

	UPROPERTY(EditDefaultsOnly, Category="Settings|Property")
	float MakingTime = 2.0f;

	/** 현재 저장된 쓰레기 개수 관련 변수*/
	UPROPERTY(EditDefaultsOnly,Replicated, Category="Settings|Property")
	float GarbageCount = 0.f;

	
	/** Making 관련 변수 */
	UPROPERTY(EditDefaultsOnly,Replicated, Category="Settings|Property")
	bool bIsMaking;

	/** 변환기 주인 설정 변수 */
	UPROPERTY(EditDefaultsOnly, Category="Settings|Property")
	class ABaseCharacter* myOwner;

	
	/** Time line을 이용한 메쉬 움직임 */
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UTimelineComponent* ConverterTimeLineComp;


public:
	// 커브 에셋을 보관하는 변수
	UPROPERTY(EditAnywhere)
	UCurveFloat* ConverterTimelineFloatCurve;

private:
	// 업데이트 트랙 이벤트를 처리할 플로트 트랙 시그니처
	class FOnTimelineFloat UpdateFunctionFloat;

	UFUNCTION()
	void UpdateTimelineComp(float Output);

	
	FMAKINGDELEGATE FmakingDelegate;

	FUNMAKINGDELEGATE Funmakingdelegate;

	UFUNCTION()
	void MakingEffect();

	UFUNCTION()
	void UnMakeingEffect();
	

};
