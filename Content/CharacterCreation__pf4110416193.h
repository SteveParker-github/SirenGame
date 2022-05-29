#pragma once
#include "Blueprint/BlueprintSupport.h"
#include "CharacterType__pf4110416193.h"
#include "Character__pf4110416193.h"
#include "Name__pf4110416193.h"
#include "CreateCrew__pf3728213905.h"
#include "Crew__pf3728213905.h"
#include "ShipBase__pf3728213905.h"
#include "Ship__pf3728213905.h"
#include "Runtime/Engine/Classes/GameFramework/GameModeBase.h"
class USceneComponent;
#include "CharacterCreation__pf4110416193.generated.h"
UCLASS(config=Game, Blueprintable, BlueprintType, meta=(ReplaceConverted="/Game/Characters/CharacterCreation.CharacterCreation_C", OverrideNativeName="CharacterCreation_C"))
class ACharacterCreation_C__pf4110416193 : public AGameModeBase
{
public:
	GENERATED_BODY()
	UPROPERTY(BlueprintReadWrite, NonTransactional, meta=(Category="Default", OverrideNativeName="DefaultSceneRoot"))
	USceneComponent* bpv__DefaultSceneRoot__pf;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(DisplayName="Character Type", Category="Default", MultiLine="true", OverrideNativeName="CharacterType"))
	E__CharacterType__pf bpv__CharacterType__pf;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(DisplayName="First Names", Category="Default", MultiLine="true", OverrideNativeName="FirstNames"))
	TArray<FString> bpv__FirstNames__pf;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(DisplayName="Last Names", Category="Default", MultiLine="true", OverrideNativeName="LastNames"))
	TArray<FString> bpv__LastNames__pf;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(DisplayName="First Name", Category="Default", MultiLine="true", OverrideNativeName="FirstName"))
	FString bpv__FirstName__pf;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(DisplayName="Last Name", Category="Default", MultiLine="true", OverrideNativeName="LastName"))
	FString bpv__LastName__pf;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(DisplayName="Health", Category="Default", MultiLine="true", OverrideNativeName="Health"))
	int32 bpv__Health__pf;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(DisplayName="Stamina", Category="Default", MultiLine="true", OverrideNativeName="Stamina"))
	int32 bpv__Stamina__pf;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(DisplayName="Value", Category="Default", MultiLine="true", OverrideNativeName="Value"))
	int32 bpv__Value__pf;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(DisplayName="Regen", Category="Default", MultiLine="true", OverrideNativeName="Regen"))
	float bpv__Regen__pf;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(DisplayName="Character", Category="Default", MultiLine="true", OverrideNativeName="Character"))
	FCharacter__pf4110416193 bpv__Character__pf;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(DisplayName="Name", Category="Default", MultiLine="true", OverrideNativeName="Name"))
	FName__pf4110416193 bpv__Name__pf;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(DisplayName="Hireable", Category="Default", MultiLine="true", OverrideNativeName="Hireable"))
	TArray<FCharacter__pf4110416193> bpv__Hireable__pf;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(DisplayName="Details", Category="Default", MultiLine="true", OverrideNativeName="Details"))
	FString bpv__Details__pf;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="Hireble Str", Category="Default", MultiLine="true", ExposeOnSpawn="true", OverrideNativeName="HirebleStr"))
	FText bpv__HirebleStr__pf;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="Class Name", Category="Default", MultiLine="true", ExposeOnSpawn="true", OverrideNativeName="ClassName"))
	FText bpv__ClassName__pf;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="Option 1", Category="Default", MultiLine="true", ExposeOnSpawn="true", OverrideNativeName="Option1"))
	FText bpv__Option1__pf;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="Option 2", Category="Default", MultiLine="true", ExposeOnSpawn="true", OverrideNativeName="Option2"))
	FText bpv__Option2__pf;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="Option 3", Category="Default", MultiLine="true", ExposeOnSpawn="true", OverrideNativeName="Option3"))
	FText bpv__Option3__pf;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(DisplayName="Hireable Archer", Category="Default", MultiLine="true", OverrideNativeName="Hireable_Archer"))
	TArray<FText> bpv__Hireable_Archer__pf;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(DisplayName="Hireable Soldier", Category="Default", MultiLine="true", OverrideNativeName="Hireable_Soldier"))
	TArray<FText> bpv__Hireable_Soldier__pf;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(DisplayName="Hireable Rower", Category="Default", MultiLine="true", OverrideNativeName="Hireable_Rower"))
	TArray<FText> bpv__Hireable_Rower__pf;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(DisplayName="Set Crew", Category="Default", MultiLine="true", OverrideNativeName="SetCrew"))
	FCreateCrew__pf3728213905 bpv__SetCrew__pf;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(DisplayName="Player Crew", Category="Default", MultiLine="true", OverrideNativeName="PlayerCrew"))
	FCrew__pf3728213905 bpv__PlayerCrew__pf;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(DisplayName="Ship Base", Category="Default", MultiLine="true", OverrideNativeName="ShipBase"))
	FShipBase__pf3728213905 bpv__ShipBase__pf;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(DisplayName="Ship", Category="Default", MultiLine="true", OverrideNativeName="Ship"))
	FShip__pf3728213905 bpv__Ship__pf;
	ACharacterCreation_C__pf4110416193(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual void PostLoadSubobjects(FObjectInstancingGraph* OuterInstanceGraph) override;
	static void __CustomDynamicClassInitialization(UDynamicClass* InDynamicClass);
	static void __StaticDependenciesAssets(TArray<FBlueprintDependencyData>& AssetsToLoad);
	static void __StaticDependencies_DirectlyUsedAssets(TArray<FBlueprintDependencyData>& AssetsToLoad);
	void bpf__ExecuteUbergraph_CharacterCreation__pf_0(int32 bpp__EntryPoint__pf);
	UFUNCTION(meta=(Comment="/** Event when play begins for this actor. */", DisplayName="BeginPlay", ToolTip="Event when play begins for this actor.", CppFromBpEvent, OverrideNativeName="ReceiveBeginPlay"))
	virtual void bpf__ReceiveBeginPlay__pf();
	UFUNCTION(BlueprintCallable, meta=(BlueprintInternalUseOnly="true", Comment="/**\t * Construction script, the place to spawn components and do other setup.\t * @note Name used in CreateBlueprint function\t */", DisplayName="Construction Script", ToolTip="Construction script, the place to spawn components and do other setup.@note Name used in CreateBlueprint function", Category, CppFromBpEvent, OverrideNativeName="UserConstructionScript"))
	virtual void bpf__UserConstructionScript__pf();
	UFUNCTION(BlueprintCallable, meta=(Category="Default", OverrideNativeName="RoundFloat"))
	virtual void bpf__RoundFloat__pf(float bpp__FloatIN__pf, /*out*/ float& bpp__FloatOUT__pf);
	UFUNCTION(BlueprintCallable, meta=(Category="Default", OverrideNativeName="CreateName"))
	virtual void bpf__CreateName__pf(/*out*/ FName__pf4110416193& bpp__name__pf);
	UFUNCTION(BlueprintCallable, meta=(Category="Default", OverrideNativeName="SetRegen"))
	virtual void bpf__SetRegen__pf(/*out*/ float& bpp__regen__pf);
	UFUNCTION(BlueprintCallable, meta=(Category="Default", OverrideNativeName="Set Stamina"))
	virtual void bpf__SetxStamina__pfT(int32 bpp__Max__pf, /*out*/ int32& bpp__Stamina__pf);
	UFUNCTION(BlueprintCallable, meta=(Category="Default", OverrideNativeName="SetHealth"))
	virtual void bpf__SetHealth__pf(int32 bpp__Max__pf, /*out*/ int32& bpp__Health__pf);
	UFUNCTION(BlueprintCallable, meta=(Category="Default", OverrideNativeName="Set Value"))
	virtual int32  bpf__SetxValue__pfT(E__CharacterType__pf bpp__CharacterType__pf, int32 bpp__HealthIN__pf, int32 bpp__StaminaIN__pf, float bpp__RegenIN__pf);
	UFUNCTION(BlueprintCallable, meta=(Category="Default", OverrideNativeName="CreateRower"))
	virtual void bpf__CreateRower__pf(FName__pf4110416193 bpp__name__pf, /*out*/ FCharacter__pf4110416193& bpp__Character__pf);
	UFUNCTION(BlueprintCallable, meta=(Category="Default", OverrideNativeName="CreateSoldier"))
	virtual void bpf__CreateSoldier__pf(FName__pf4110416193 bpp__name__pf, /*out*/ FCharacter__pf4110416193& bpp__Character__pf);
	UFUNCTION(BlueprintCallable, meta=(Category="Default", OverrideNativeName="CreateCaptain"))
	virtual void bpf__CreateCaptain__pf(FName__pf4110416193 bpp__name__pf, /*out*/ FCharacter__pf4110416193& bpp__Character__pf);
	UFUNCTION(BlueprintCallable, meta=(Category="Default", OverrideNativeName="CreateArcher"))
	virtual void bpf__CreateArcher__pf(FName__pf4110416193 bpp__name__pf, /*out*/ FCharacter__pf4110416193& bpp__Character__pf);
	UFUNCTION(BlueprintCallable, meta=(Category="Default", OverrideNativeName="Create Player"))
	virtual void bpf__CreatexPlayer__pfT(/*out*/ FCharacter__pf4110416193& bpp__Character__pf);
	UFUNCTION(BlueprintCallable, meta=(Category="Default", OverrideNativeName="CreateCharacter"))
	virtual void bpf__CreateCharacter__pf(E__CharacterType__pf bpp__CharacterType__pf, /*out*/ FCharacter__pf4110416193& bpp__Character__pf);
	UFUNCTION(BlueprintCallable, meta=(Category="Default", OverrideNativeName="CreateCrew"))
	virtual void bpf__CreateCrew__pf(FCreateCrew__pf3728213905 bpp__CrewValues__pf, /*out*/ FCrew__pf3728213905& bpp__CrewOut__pf);
	UFUNCTION(BlueprintCallable, meta=(Category="Default", OverrideNativeName="Set Hireable"))
	virtual void bpf__SetxHireable__pfT(/*out*/ TArray<FCharacter__pf4110416193>& bpp__Hireable__pf);
	UFUNCTION(BlueprintCallable, meta=(Category="Default", OverrideNativeName="SetHireableStr"))
	virtual void bpf__SetHireableStr__pf(/*out*/ TArray<FCharacter__pf4110416193>& bpp__Array__pf);
	UFUNCTION(BlueprintCallable, meta=(Category="Default", OverrideNativeName="SetHireableTextBlocks"))
	virtual void bpf__SetHireableTextBlocks__pf();
	UFUNCTION(BlueprintCallable, meta=(Category, OverrideNativeName="CreateHireable"))
	virtual void bpf__CreateHireable__pf();
public:
};
