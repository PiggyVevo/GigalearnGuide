/* IMPORTANT!!!!!!!!!!!!!!!!!!!
Just replace this with your original examplemain.cpp file in your GigalearnCPP-Leak/src file
	*/

#include <GigaLearnCPP/Learner.h>

#include <RLGymCPP/Rewards/CommonRewards.h>
#include <RLGymCPP/Rewards/ZeroSumReward.h>
#include <RLGymCPP/TerminalConditions/NoTouchCondition.h>
#include <RLGymCPP/TerminalConditions/GoalScoreCondition.h>
#include <RLGymCPP/OBSBuilders/DefaultObsPadded.h>
#include <RLGymCPP/StateSetters/KickoffState.h>
#include <RLGymCPP/StateSetters/RandomState.h>
#include <RLGymCPP/StateSetters/CombinedState.h>
#include <RLGymCPP/ActionParsers/DefaultAction.h>
//include all of our directories to compile the Gigalearnbot.exe
using namespace GGL;
using namespace RLGC;

// Helper to randomly choose team size (1v1, 2v2, or 3v3)
int GetRandomTeamSize() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dist(0, 2);
    int r = dist(gen);
    return r == 0 ? 1 : (r == 1 ? 2 : 3);
}

EnvCreateResult EnvCreateFunc(int index) {
    std::vector<WeightedReward> rewards = {
        { new AirReward(), 0.15f },
        { new FaceBallReward(), 0.75f }, 
        { new ZeroSumReward(new VelocityPlayerToBallReward(), 1.0f, 0.0f), 5.0f},		
        { new ZeroSumReward(new StrongTouchReward(), 1.0f, 1.0f), 50.0f },
        // { new ZeroSumReward(new VelocityBallToGoalReward(), 1.0f), 75.0f },
		    // { new ZeroSumReward(new PickupBoostReward(), 1.0f, 1.0f), 100.0f},
        // { new SaveBoostReward(), 1.0f },
        // { new ZeroSumReward(new BumpReward(), 1.0f), 400.0f },
        // { new ZeroSumReward(new DemoReward(), 1.0f), 400.0f },
        // { new WavedashReward(), 20.0f },
        { new GoalReward(), 500.0f }
    };
    // these rewards will get your bot to touch the ball pretty fast, the rewards I commented out are commented out FOR A REASON dont use the oommented out rewards at first
    std::vector<TerminalCondition*> terminalConditions = {
        new NoTouchCondition(15),
        new GoalScoreCondition()
    };

    // Random team size: 1, 2, or 3 players per team
    int teamSize = GetRandomTeamSize();
    int playersPerTeam = teamSize;
    auto arena = Arena::Create(GameMode::SOCCAR);
    
    // Use PLANK hitbox (Batmobile) you can use octane to, just change it to CAR_CONFIG_OCTANE
    for (int i = 0; i < playersPerTeam; ++i) {
        arena->AddCar(Team::BLUE, CAR_CONFIG_PLANK);
        arena->AddCar(Team::ORANGE, CAR_CONFIG_PLANK);
    }

    std::vector<std::pair<StateSetter*, float>> weightedSetters = {
        { new KickoffState(), 0.5f },
        { new RandomState(true, true, true), 0.5f },
    }; //state setters, kickoff and randomstate weights go tune them yourself
    CombinedState* combinedSetter = new CombinedState(weightedSetters);

    // Padded observation builder for up to 3 players per team
    auto obsBuilder = new DefaultObsPadded(3);
    auto actionParser = new DefaultAction();

    EnvCreateResult result = {};
    result.actionParser = actionParser;
    result.obsBuilder = obsBuilder;
    result.stateSetter = combinedSetter;
    result.terminalConditions = terminalConditions;
    result.rewards = rewards;
    result.arena = arena;

    return result;
}

void StepCallback(Learner* learner, const std::vector<GameState>& states, Report& report) {
    bool doExpensiveMetrics = (rand() % 4) == 0;
    for (auto& state : states) {
        if (doExpensiveMetrics) {
            for (auto& player : state.players) {
                report.AddAvg("Player/In Air Ratio", !player.isOnGround);
                report.AddAvg("Player/Ball Touch Ratio", player.ballTouchedStep);
                report.AddAvg("Player/Demoed Ratio", player.isDemoed);
                report.AddAvg("Player/Speed", player.vel.Length());
                Vec dirToBall = (state.ball.pos - player.pos).Normalized();
                report.AddAvg("Player/Speed Towards Ball", RS_MAX(0, player.vel.Dot(dirToBall)));
                report.AddAvg("Player/Boost", player.boost);
                if (player.ballTouchedStep)
                    report.AddAvg("Player/Touch Height", state.ball.pos.z);
            }
        }
        if (state.goalScored)
            report.AddAvg("Game/Goal Speed", state.ball.vel.Length());
    }
} //them metrics, I dont use metrics tho

int main(int argc, char* argv[]) {
    RocketSim::Init("collision_meshes"); //INCLUDE YOUR COLLISION MESHES

    LearnerConfig cfg = {};
    cfg.deviceType = LearnerDeviceType::CPU;
    cfg.tickSkip = 8; //tick skip, chaging this will change gamma
    cfg.actionDelay = cfg.tickSkip - 1;
    cfg.numGames = 128; //adjust to how good your cpu is, mine is a i7-12700k and 192 games is optimal for me. The better your cpu is, the more games you should have.

    cfg.ppo.tsPerItr = 50000;  
    cfg.ppo.batchSize = 50000; //how much your bot trains at a time
    cfg.ppo.miniBatchSize = 50000; //minibatch size. if you have small pc, 25k is good, if you have a powerful pc (4070ti or better) 75k might be optimal
    cfg.ppo.epochs = 1; //start out with one epoch, and once your bot gets better increase this to two
    cfg.ppo.entropyScale = 0.035f; //this is a good starting point, lower it if your bot is like very good, or you just want to refine what it already knows                              
    cfg.ppo.gaeGamma = 0.99f; //start with .99, then up to .993 once it can hit ball and shoot ball on net, then .995 once it learns dribbles and powershots, .997 once it gets better than necto.
    cfg.ppo.policyLR = 2e-4f;
    cfg.ppo.criticLR = 2e-4f; //learning rates. start out high, then lower to 1.5e-4 when it learns to shoot and touch ball, then 1e-4 once it learns dribbles, then 0.8e-4 once it is around nexto level.

    cfg.ppo.sharedHead.layerSizes = { 512, 512, 512}; //your bot has a shared head, both the cpu and critic learn from this, this should be big sizes
    cfg.ppo.policy.layerSizes = { 256, 256, 256 };
    cfg.ppo.critic.layerSizes = { 256, 256, 256 }; //these are pretty good, DO NOT INCREASE IT FURTHER

    cfg.ppo.sharedHead.activationType = ModelActivationType::LEAKY_RELU; //leakly relu prevent nuerons from going dead, but slows down training a bit
    cfg.ppo.policy.activationType = ModelActivationType::LEAKY_RELU; 
    cfg.ppo.critic.activationType = ModelActivationType::LEAKY_RELU;

    cfg.ppo.sharedHead.addLayerNorm = true; //dont touch
    cfg.ppo.policy.addLayerNorm = true;
    cfg.ppo.critic.addLayerNorm = true;

    cfg.skillTracker.enabled = true; //quite useful for some, but I dont really look at graphs. Turn it off if you dont want to be like JeffA233 xD
    cfg.skillTracker.numArenas = 8;
    cfg.skillTracker.simTime = 45;
    cfg.skillTracker.updateInterval = 16;
    cfg.skillTracker.ratingInc = 5;
    cfg.skillTracker.initialRating = 0;

    cfg.checkpointFolder = "GGLRUN"; //the name of your bots run

    bool renderMode = false;
    for (int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) == "--render") { //render tag, you use it like Gigalearnbot.exe --render
            renderMode = true;
            break;
        }
    }

    if (renderMode) { //render stuff
        cfg.renderMode = true;
        cfg.renderTimeScale = 1.0f;
        cfg.sendMetrics = false;
        cfg.numGames = 1;
        cfg.ppo.deterministic = true;
        cfg.skillTracker.enabled = false;
    } else {
        cfg.sendMetrics = true; //whether to send metrics 
    }

    cfg.randomSeed = 123; // use -1 for random seed for viewing, this doesnt matter tho

    Learner* learner = new Learner(EnvCreateFunc, cfg, StepCallback);
    learner->Start();

    return EXIT_SUCCESS;
}
