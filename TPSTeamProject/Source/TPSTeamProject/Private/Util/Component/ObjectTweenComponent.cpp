#include "Util/Component/ObjectTweenComponent.h"
#include "Components/TimelineComponent.h"

UObjectTweenComponent::UObjectTweenComponent()
	: ValueType(EObjectTweenValueType::Float)
	, CurveFloat(nullptr)
	, CurveVector(nullptr)
	, CurveLinearColor(nullptr)
{
	PrimaryComponentTick.bCanEverTick = false;

	TweenTimelineComp = CreateDefaultSubobject<UTimelineComponent>(TEXT("TweenTimelineComp"));
}


void UObjectTweenComponent::AddFloatEvent(UObject* InObject, const FName& UpdateFunctionName, const FName& FinishedFunctionName)
{
	checkf(ValueType == EObjectTweenValueType::Float, TEXT("This function can only be called if the ValueType of the ObjectTweenComponent is EObjectTweenValueType::Float."));
	checkf(CurveFloat, TEXT("You must set the CurveFloat in the Blueprint Editor."));

	FOnTimelineFloat TimelineProgress;
	TimelineProgress.BindUFunction(InObject, UpdateFunctionName);

	TweenTimelineComp->AddInterpFloat(CurveFloat, TimelineProgress);

	if (FinishedFunctionName != NAME_None)
	{
		FOnTimelineEventStatic TimelineFinished;
		TimelineFinished.BindUFunction(InObject, FinishedFunctionName);

		TweenTimelineComp->SetTimelineFinishedFunc(TimelineFinished);
	}
}

void UObjectTweenComponent::AddVectorEvent(UObject* InObject, const FName& UpdateFunctionName, const FName& FinishedFunctionName)
{
	checkf(ValueType == EObjectTweenValueType::Vector, TEXT("This function can only be called if the ValueType of the ObjectTweenComponent is EObjectTweenValueType::Vector."));
	checkf(CurveVector, TEXT("You must set the CurveVector in the Blueprint Editor."));

	FOnTimelineVector TimelineProgress;
	TimelineProgress.BindUFunction(InObject, UpdateFunctionName);

	TweenTimelineComp->AddInterpVector(CurveVector, TimelineProgress);

	if (FinishedFunctionName != NAME_None)
	{
		FOnTimelineEventStatic TimelineFinished;
		TimelineFinished.BindUFunction(InObject, FinishedFunctionName);

		TweenTimelineComp->SetTimelineFinishedFunc(TimelineFinished);
	}
}

void UObjectTweenComponent::AddLinearColorEvent(UObject* InObject, const FName& UpdateFunctionName, const FName& FinishedFunctionName)
{
	checkf(ValueType == EObjectTweenValueType::LinearColor, TEXT("This function can only be called if the ValueType of the ObjectTweenComponent is EObjectTweenValueType::LinearColor."));
	checkf(CurveLinearColor, TEXT("You must set the CurveLinearColor in the Blueprint Editor."));

	FOnTimelineLinearColor TimelineProgress;
	TimelineProgress.BindUFunction(InObject, UpdateFunctionName);

	TweenTimelineComp->AddInterpLinearColor(CurveLinearColor, TimelineProgress);

	if (FinishedFunctionName != NAME_None)
	{
		FOnTimelineEventStatic TimelineFinished;
		TimelineFinished.BindUFunction(InObject, FinishedFunctionName);

		TweenTimelineComp->SetTimelineFinishedFunc(TimelineFinished);
	}
}

void UObjectTweenComponent::AddBoolEvent(UObject* InObject, const FName& UpdateFunctionName, float InTime)
{
	checkf(ValueType == EObjectTweenValueType::Boolean, TEXT("This function can only be called if the ValueType of the ObjectTweenComponent is EObjectTweenValueType::Boolean."));

	FOnTimelineEvent TweenEvent;
	TweenEvent.BindUFunction(InObject, UpdateFunctionName);

	TweenTimelineComp->AddEvent(InTime, TweenEvent);
}

void UObjectTweenComponent::PlayFromStart()
{
	//Super::PlayFromStart();

	if (IsValid(TweenTimelineComp) == false)
	{
		return;
	}

	TweenTimelineComp->PlayFromStart();
}

void UObjectTweenComponent::Reverse()
{
	if (IsValid(TweenTimelineComp) == false)
	{
		return;
	}

	TweenTimelineComp->Reverse();
}

void UObjectTweenComponent::EndPlay(EEndPlayReason::Type InType)
{
	Super::EndPlay(InType);

	if (TweenTimelineComp->HasBegunPlay() == false)
	{
		return;
	}

	TweenTimelineComp->EndPlay(InType);
}

// Called when the game starts
void UObjectTweenComponent::BeginPlay()
{
	Super::BeginPlay();

	
}

