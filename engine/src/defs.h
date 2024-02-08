#pragma once
#include <chrono>
#include <stdint.h>

namespace Colors {
constexpr uint8_t White = 0;
constexpr uint8_t Black = 1;
}; // namespace Colors
namespace Pieces {
constexpr uint8_t Blank = 0;
constexpr uint8_t WPawn = 2;
constexpr uint8_t BPawn = 3;
constexpr uint8_t WKnight = 4;
constexpr uint8_t BKnight = 5;
constexpr uint8_t WBishop = 6;
constexpr uint8_t BBishop = 7;
constexpr uint8_t WRook = 8;
constexpr uint8_t BRook = 9;
constexpr uint8_t WQueen = 10;
constexpr uint8_t BQueen = 11;
constexpr uint8_t WKing = 12;
constexpr uint8_t BKing = 13;
}; // namespace Pieces
namespace Directions {
constexpr int8_t North = 16;
constexpr int8_t South = -16;
constexpr int8_t East = 1;
constexpr int8_t West = -1;
constexpr int8_t Northeast = 17;
constexpr int8_t Southeast = -15;
constexpr int8_t Northwest = 15;
constexpr int8_t Southwest = -17;
} // namespace Directions
namespace Sides {
constexpr int8_t Kingside = 1;
constexpr int8_t Queenside = 0;
} // namespace Sides

namespace EntryTypes {
constexpr uint8_t None = 0;
constexpr uint8_t UBound = 1;
constexpr uint8_t LBound = 2;
constexpr uint8_t Exact = 3;
} // namespace EntryTypes


namespace Promos {
  constexpr uint8_t Knight = 0;
  constexpr uint8_t Bishop = 1;
  constexpr uint8_t Rook = 2;
  constexpr uint8_t Queen = 3;
}

constexpr int16_t ListSize = 216;
constexpr int16_t GameSize = 2000;
constexpr int32_t ScoreNone = INT32_MIN;
constexpr int32_t Mate = -100000;
constexpr int32_t MateScore = 80000;
typedef uint32_t Move;
/*The format of a move structure is:      from     to      promo
                                         (<< 10)  (<< 2)
                                        xxxxxxxx xxxxxxxx  xx
                                        */

constexpr Move MoveNone = 0;

struct MoveInfo {
  Move moves[ListSize];
  int scores[ListSize];
};

struct Position {
  uint8_t board[0x80];        // Stores the board itself
  uint8_t material_count[10]; // Stores material
  bool castling_rights[2][2]; // castling rights
  uint8_t kingpos[2];         // Stores King positions
  uint8_t ep_square;          // stores ep square
  bool color;                 // whose side to move
  uint8_t halfmoves;
};

struct GameHistory { // keeps the state of the board at a particular point in
                     // the game
  uint64_t position_key; // Hash key of the position at the time
  Move played_move;      // The move that was played
  uint8_t piece_moved; // The piece that was moved (will be useful for histories
                       // later)
};

struct ThreadInfo {
  uint64_t zobrist_key; // hash key of the position we're currently on
  uint16_t thread_id;   // ID of the thread
  GameHistory game_hist[GameSize]; // all positions from earlier in the game
  uint16_t game_ply;               // how far we're into the game
  uint16_t search_ply;             // depth that we are in the search tree
  uint64_t nodes;                  // Total nodes searched so far this search
  std::chrono::_V2::steady_clock::time_point
      start_time; // Start time of the search
};

constexpr int MaxSearchDepth = 127;

struct TTEntry {
  uint32_t position_key; // The upper 32 bits of the hash key are stored
  uint8_t depth;         // Depth that the entry was searched to
  Move best_move;        // Best move in the position
  int32_t score;         // Score of the position
  uint8_t type;          // entry type
};

constexpr int StandardToMailbox[64] =
    { // Used to convert standard board position into mailbox position
        0x0,  0x1,  0x2,  0x3,  0x4,  0x5,  0x6,  0x7,  0x10, 0x11, 0x12,
        0x13, 0x14, 0x15, 0x16, 0x17, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25,
        0x26, 0x27, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x40,
        0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x50, 0x51, 0x52, 0x53,
        0x54, 0x55, 0x56, 0x57, 0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66,
        0x67, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77};

constexpr int MailboxToStandard_NNUE[0x80] = {
    // Needed for NNUE inference because trainers use 0 = a8, while my board has
    // 0 = a1.
    56, 57, 58, 59, 60, 61, 62, 63, 99, 99, 99, 99, 99, 99, 99, 99, 48, 49, 50,
    51, 52, 53, 54, 55, 99, 99, 99, 99, 99, 99, 99, 99, 40, 41, 42, 43, 44, 45,
    46, 47, 99, 99, 99, 99, 99, 99, 99, 99, 32, 33, 34, 35, 36, 37, 38, 39, 99,
    99, 99, 99, 99, 99, 99, 99, 24, 25, 26, 27, 28, 29, 30, 31, 99, 99, 99, 99,
    99, 99, 99, 99, 16, 17, 18, 19, 20, 21, 22, 23, 99, 99, 99, 99, 99, 99, 99,
    99, 8,  9,  10, 11, 12, 13, 14, 15, 99, 99, 99, 99, 99, 99, 99, 99, 0,  1,
    2,  3,  4,  5,  6,  7,  99, 99, 99, 99, 99, 99, 99, 99,
};

constexpr int MailboxToStandard[0x80] = {
    // Used to convert mailbox position into standard position, useful for hash
    // keys etc.
    0,  1,  2,  3,  4,  5,  6,  7,  99, 99, 99, 99, 99, 99, 99, 99, 8,  9,  10,
    11, 12, 13, 14, 15, 99, 99, 99, 99, 99, 99, 99, 99, 16, 17, 18, 19, 20, 21,
    22, 23, 99, 99, 99, 99, 99, 99, 99, 99, 24, 25, 26, 27, 28, 29, 30, 31, 99,
    99, 99, 99, 99, 99, 99, 99, 32, 33, 34, 35, 36, 37, 38, 39, 99, 99, 99, 99,
    99, 99, 99, 99, 40, 41, 42, 43, 44, 45, 46, 47, 99, 99, 99, 99, 99, 99, 99,
    99, 48, 49, 50, 51, 52, 53, 54, 55, 99, 99, 99, 99, 99, 99, 99, 99, 56, 57,
    58, 59, 60, 61, 62, 63, 99, 99, 99, 99, 99, 99, 99, 99,
};

constexpr int8_t AttackRays[8] = {
    Directions::East,      Directions::West, // Directions sliders can move in
    Directions::South,     Directions::North,     Directions::Southeast,
    Directions::Southwest, Directions::Northeast, Directions::Northwest};

constexpr int8_t KnightAttacks[8] = {
    Directions::East * 2 + Directions::North, // Directions Knights can move in
    Directions::East * 2 + Directions::South,
    Directions::South * 2 + Directions::East,
    Directions::South * 2 + Directions::West,
    Directions::West * 2 + Directions::South,
    Directions::West * 2 + Directions::North,
    Directions::North * 2 + Directions::West,
    Directions::North * 2 + Directions::East};

constexpr int8_t SliderAttacks[4][8] =
    { // Attack vectors for bishops through kings
        {Directions::Southeast, Directions::Southwest, Directions::Northeast,
         Directions::Northwest},
        {Directions::East, Directions::West, Directions::South,
         Directions::North},
        {Directions::East, Directions::West, Directions::South,
         Directions::North, Directions::Southeast, Directions::Southwest,
         Directions::Northeast, Directions::Northwest},
        {Directions::East, Directions::West, Directions::South,
         Directions::North, Directions::Southeast, Directions::Southwest,
         Directions::Northeast, Directions::Northwest}};


constexpr int SeeValues[14] = {
  0, 0, 100, 100, 450, 450, 650, 650, 1250, 1250, 10000, 10000
};

#define out_of_board(x) (x & 0x88)
#define get_rank(x) (x / 16)
#define get_file(x) (x % 16)
#define flip_sq(x) (x ^ 112)
#define get_color(x) (x & 1)
#define pack_move(from, to, promo) ((from << 10) + (to << 2) + promo)
#define extract_from(move) (move >> 10)
#define extract_to(move) ((move >> 2) & 127)
#define extract_promo(move) (move & 3)
#define friendly_square(color, piece) (piece && (piece & 1) == color)
#define enemy_square(color, piece) (piece && (piece & 1) != color)
#define get_zobrist_key(piece, sq) (((piece - 2) * 64) + sq)
#define get_hash_upper_bits(hash) (hash >> 32)
#define standard(mailbox) (MailboxToStandard[mailbox])

constexpr int32_t side_index = 772;
constexpr int32_t ep_index = 773;
constexpr int32_t castling_index = 774;

constexpr uint64_t zobrist_keys[778] = {
    // Zobrist hash keys taken from Willow.
    // Their usage:
    // 64 * piece (0-11) + sq
    // 772 - side to move
    // 773 - en passant possible
    // 774 - 777 - KQkq castling rights
    7266447313870364031ull,  4946485549665804864ull,  16945909448695747420ull,
    16394063075524226720ull, 4873882236456199058ull,  14877448043947020171ull,
    6740343660852211943ull,  13857871200353263164ull, 5249110015610582907ull,
    10205081126064480383ull, 1235879089597390050ull,  17320312680810499042ull,
    16489141110565194782ull, 8942268601720066061ull,  13520575722002588570ull,
    14226945236717732373ull, 9383926873555417063ull,  15690281668532552105ull,
    11510704754157191257ull, 15864264574919463609ull, 6489677788245343319ull,
    5112602299894754389ull,  10828930062652518694ull, 15942305434158995996ull,
    15445717675088218264ull, 4764500002345775851ull,  14673753115101942098ull,
    236502320419669032ull,   13670483975188204088ull, 14931360615268175698ull,
    8904234204977263924ull,  12836915408046564963ull, 12120302420213647524ull,
    15755110976537356441ull, 5405758943702519480ull,  10951858968426898805ull,
    17251681303478610375ull, 4144140664012008120ull,  18286145806977825275ull,
    13075804672185204371ull, 10831805955733617705ull, 6172975950399619139ull,
    12837097014497293886ull, 12903857913610213846ull, 560691676108914154ull,
    1074659097419704618ull,  14266121283820281686ull, 11696403736022963346ull,
    13383246710985227247ull, 7132746073714321322ull,  10608108217231874211ull,
    9027884570906061560ull,  12893913769120703138ull, 15675160838921962454ull,
    2511068401785704737ull,  14483183001716371453ull, 3774730664208216065ull,
    5083371700846102796ull,  9583498264570933637ull,  17119870085051257224ull,
    5217910858257235075ull,  10612176809475689857ull, 1924700483125896976ull,
    7171619684536160599ull,  10949279256701751503ull, 15596196964072664893ull,
    14097948002655599357ull, 615821766635933047ull,   5636498760852923045ull,
    17618792803942051220ull, 580805356741162327ull,   425267967796817241ull,
    8381470634608387938ull,  13212228678420887626ull, 16993060308636741960ull,
    957923366004347591ull,   6210242862396777185ull,  1012818702180800310ull,
    15299383925974515757ull, 17501832009465945633ull, 17453794942891241229ull,
    15807805462076484491ull, 8407189590930420827ull,  974125122787311712ull,
    1861591264068118966ull,  997568339582634050ull,   18046771844467391493ull,
    17981867688435687790ull, 3809841506498447207ull,  9460108917638135678ull,
    16172980638639374310ull, 958022432077424298ull,   4393365126459778813ull,
    13408683141069553686ull, 13900005529547645957ull, 15773550354402817866ull,
    16475327524349230602ull, 6260298154874769264ull,  12224576659776460914ull,
    6405294864092763507ull,  7585484664713203306ull,  5187641382818981381ull,
    12435998400285353380ull, 13554353441017344755ull, 646091557254529188ull,
    11393747116974949255ull, 16797249248413342857ull, 15713519023537495495ull,
    12823504709579858843ull, 4738086532119935073ull,  4429068783387643752ull,
    585582692562183870ull,   1048280754023674130ull,  6788940719869959076ull,
    11670856244972073775ull, 2488756775360218862ull,  2061695363573180185ull,
    6884655301895085032ull,  3566345954323888697ull,  12784319933059041817ull,
    4772468691551857254ull,  6864898938209826895ull,  7198730565322227090ull,
    2452224231472687253ull,  13424792606032445807ull, 10827695224855383989ull,
    11016608897122070904ull, 14683280565151378358ull, 7077866519618824360ull,
    17487079941198422333ull, 3956319990205097495ull,  5804870313319323478ull,
    8017203611194497730ull,  3310931575584983808ull,  5009341981771541845ull,
    6930001938490791874ull,  14415278059151389495ull, 11001114762641844083ull,
    6715939435439735925ull,  411419160297131328ull,   4522402260441335284ull,
    3381955501804126859ull,  15935778656111987797ull, 4345051260540166684ull,
    13978444093099579683ull, 9219789505504949817ull,  9245142924137529075ull,
    11628184459157386459ull, 7242398879359936370ull,  8511401943157540109ull,
    11948130810477009827ull, 6865450671488705049ull,  13965005347172621081ull,
    15956599226522058336ull, 7737868921014130584ull,  2107342503741411693ull,
    15818996300425101108ull, 16399939197527488760ull, 13971145494081508107ull,
    3910681448359868691ull,  4249175367970221090ull,  9735751321242454020ull,
    12418107929362160460ull, 241792245481991138ull,   5806488997649497146ull,
    10724207982663648949ull, 1121862814449214435ull,  1326996977123564236ull,
    4902706567834759475ull,  12782714623891689967ull, 7306216312942796257ull,
    15681656478863766664ull, 957364844878149318ull,   5651946387216554503ull,
    8197027112357634782ull,  6302075516351125977ull,  13454588464089597862ull,
    15638309200463515550ull, 10116604639722073476ull, 12052913535387714920ull,
    2889379661594013754ull,  15383926144832314187ull, 7841953313015471731ull,
    17310575136995821873ull, 9820021961316981626ull,  15319619724109527290ull,
    15349724127275899898ull, 10511508162402504492ull, 6289553862380300393ull,
    15046218882019267110ull, 11772020174577005930ull, 3537640779967351792ull,
    6801855569284252424ull,  17687268231192623388ull, 12968358613633237218ull,
    1429775571144180123ull,  10427377732172208413ull, 12155566091986788996ull,
    16465954421598296115ull, 12710429690464359999ull, 9547226351541565595ull,
    12156624891403410342ull, 2985938688676214686ull,  18066917785985010959ull,
    5975570403614438776ull,  11541343163022500560ull, 11115388652389704592ull,
    9499328389494710074ull,  9247163036769651820ull,  3688303938005101774ull,
    2210483654336887556ull,  15458161910089693228ull, 6558785204455557683ull,
    1288373156735958118ull,  18433986059948829624ull, 3435082195390932486ull,
    16822351800343061990ull, 3120532877336962310ull,  16681785111062885568ull,
    7835551710041302304ull,  2612798015018627203ull,  15083279177152657491ull,
    6591467229462292195ull,  10592706450534565444ull, 7438147750787157163ull,
    323186165595851698ull,   7444710627467609883ull,  8473714411329896576ull,
    2782675857700189492ull,  3383567662400128329ull,  3200233909833521327ull,
    12897601280285604448ull, 3612068790453735040ull,  8324209243736219497ull,
    15789570356497723463ull, 1083312926512215996ull,  4797349136059339390ull,
    5556729349871544986ull,  18266943104929747076ull, 1620389818516182276ull,
    172225355691600141ull,   3034352936522087096ull,  1266779576738385285ull,
    3906668377244742888ull,  6961783143042492788ull,  17159706887321247572ull,
    4676208075243319061ull,  10315634697142985816ull, 13435140047933251189ull,
    716076639492622016ull,   13847954035438697558ull, 7195811275139178570ull,
    10815312636510328870ull, 6214164734784158515ull,  16412194511839921544ull,
    3862249798930641332ull,  1005482699535576005ull,  4644542796609371301ull,
    17600091057367987283ull, 4209958422564632034ull,  5419285945389823940ull,
    11453701547564354601ull, 9951588026679380114ull,  7425168333159839689ull,
    8436306210125134906ull,  11216615872596820107ull, 3681345096403933680ull,
    5770016989916553752ull,  11102855936150871733ull, 11187980892339693935ull,
    396336430216428875ull,   6384853777489155236ull,  7551613839184151117ull,
    16527062023276943109ull, 13429850429024956898ull, 9901753960477271766ull,
    9731501992702612259ull,  5217575797614661659ull,  10311708346636548706ull,
    15111747519735330483ull, 4353415295139137513ull,  1845293119018433391ull,
    11952006873430493561ull, 3531972641585683893ull,  16852246477648409827ull,
    15956854822143321380ull, 12314609993579474774ull, 16763911684844598963ull,
    16392145690385382634ull, 1545507136970403756ull,  17771199061862790062ull,
    12121348462972638971ull, 12613068545148305776ull, 954203144844315208ull,
    1257976447679270605ull,  3664184785462160180ull,  2747964788443845091ull,
    15895917007470512307ull, 15552935765724302120ull, 16366915862261682626ull,
    8385468783684865323ull,  10745343827145102946ull, 2485742734157099909ull,
    916246281077683950ull,   15214206653637466707ull, 12895483149474345798ull,
    1079510114301747843ull,  10718876134480663664ull, 1259990987526807294ull,
    8326303777037206221ull,  14104661172014248293ull, 15531278677382192198ull,
    3874303698666230242ull,  3611366553819264523ull,  1358753803061653874ull,
    1552102816982246938ull,  14492630642488100979ull, 15001394966632908727ull,
    2273140352787320862ull,  17843678642369606172ull, 2903980458593894032ull,
    16971437123015263604ull, 12969653681729206264ull, 3593636458822318001ull,
    9719758956915223015ull,  7437601263394568346ull,  3327758049015164431ull,
    17851524109089292731ull, 14769614194455139039ull, 8017093497335662337ull,
    12026985381690317404ull, 739616144640253634ull,   15535375191850690266ull,
    2418267053891303448ull,  15314073759564095878ull, 10333316143274529509ull,
    16565481511572123421ull, 16317667579273275294ull, 13991958187675987741ull,
    3753596784796798785ull,  9078249094693663275ull,  8459506356724650587ull,
    12579909555010529099ull, 7827737296967050903ull,  5489801927693999341ull,
    10995988997350541459ull, 14721747867313883304ull, 7915884580303296560ull,
    4105766302083365910ull,  12455549072515054554ull, 13602111324515032467ull,
    5205971628932290989ull,  5034622965420036444ull,  9134927878875794005ull,
    11319873529597990213ull, 14815445109496752058ull, 2266601052460299470ull,
    5696993487088103383ull,  6540200741841280242ull,  6631495948031875490ull,
    5328340585170897740ull,  17897267040961463930ull, 9030000260502624168ull,
    14285709137129830926ull, 12854071997824681544ull, 15408328651008978682ull,
    1063314403033437073ull,  13765209628446252802ull, 242013711116865605ull,
    4772374239432528212ull,  2515855479965038648ull,  5872624715703151235ull,
    14237704570091006662ull, 678604024776645862ull,   12329607334079533339ull,
    17570877682732917020ull, 2695443415284373666ull,  4312672841405514468ull,
    6454343485137106900ull,  8425658828390111343ull,  16335501385875554899ull,
    5551095603809016713ull,  11781094401885925035ull, 9395557946368382509ull,
    9765123360948816956ull,  18107191819981188154ull, 16049267500594757404ull,
    16349966108299794199ull, 1040405303135858246ull,  2366386386131378192ull,
    223761048139910454ull,   15375217587047847934ull, 15231693398695187454ull,
    12916726640254571028ull, 8878036960829635584ull,  1626201782473074365ull,
    5758998126998248293ull,  18077917959300292758ull, 10585588923088536745ull,
    15072345664541731497ull, 3559348759319842667ull,  12744591691872202375ull,
    2388494115860283059ull,  6414691845696331748ull,  3069528498807764495ull,
    8737958486926519702ull,  18059264986425101074ull, 3139684427605102737ull,
    12378931902986734693ull, 410666675039477949ull,   12139894855769838924ull,
    5780722552400398675ull,  7039346665375142557ull,  3020733445712569008ull,
    2612305843503943561ull,  13651771214166527665ull, 16478681918975800939ull,
    566088527565499576ull,   4715785502295754870ull,  6957318344287196220ull,
    11645756868405128885ull, 13139951104358618000ull, 17650948583490040612ull,
    18168787973649736637ull, 5486282999836125542ull,  6122201977153895166ull,
    17324241605502052782ull, 10063523107521105867ull, 17537430712468011382ull,
    10828407533637104262ull, 10294139354198325113ull, 12557151830240236401ull,
    16673044307512640231ull, 10918020421896090419ull, 11077531235278014145ull,
    5499571814940871256ull,  2334252435740638702ull,  18177461912527387031ull,
    2000007376901262542ull,  7968425560071444214ull,  1472650787501520648ull,
    3115849849651526279ull,  7980970700139577536ull,  12153253535907642097ull,
    8109716914843248719ull,  3154976533165008908ull,  5553369513523832559ull,
    10345792701798576501ull, 3677445364544507875ull,  10637177623943913351ull,
    7380255087060498096ull,  14479400372337014801ull, 15381362583330700960ull,
    204531043189704802ull,   13699106540959723942ull, 3817903465872254783ull,
    10972364467110284934ull, 2701394334530963810ull,  2931625600749229147ull,
    16428252083632828910ull, 11873166501966812913ull, 5566810080537233762ull,
    7840617383807795056ull,  10699413880206684652ull, 18259119259617231436ull,
    10332714341486317526ull, 10137911902863059694ull, 669146221352346842ull,
    8373571610024623455ull,  10620002450820868661ull, 12220730820779815970ull,
    5902974968095412898ull,  7931010481705150841ull,  16413777368097063650ull,
    11273457888324769727ull, 13719113891065284171ull, 8327795098009702553ull,
    10333342364827584837ull, 6202832891413866653ull,  9137034567886143162ull,
    14514450826524340059ull, 473610156015331016ull,   813689571029117640ull,
    13776316799690285717ull, 10429708855338427756ull, 8995290140880620858ull,
    2320123852041754384ull,  8082864073645003641ull,  6961777411740398590ull,
    10008644283003991179ull, 3239064015890722333ull,  16762634970725218787ull,
    16467281536733948427ull, 10563290046315192938ull, 5108560603794851559ull,
    15121667220761532906ull, 14155440077372845941ull, 10050536352394623377ull,
    15474881667376037792ull, 3448088038819200619ull,  3692020001240358871ull,
    6444847992258394902ull,  8687650838094264665ull,  3028124591188972359ull,
    16945232313401161629ull, 15547830510283682816ull, 3982930188609442149ull,
    14270781928849894661ull, 13768475593433447867ull, 13815150225221307677ull,
    8502397232429564693ull,  718377350715476994ull,   7459266877697905475ull,
    8353375565171101521ull,  7807281661994435472ull,  16924127046922196149ull,
    10157812396471387805ull, 2519858716882670232ull,  7384148884750265792ull,
    8077153156180046901ull,  3499231286164597752ull,  2700106282881469611ull,
    14679824700835879737ull, 14188324938219126828ull, 3016120398601032793ull,
    10858152824243889420ull, 9412371965669250534ull,  4857522662584941069ull,
    984331743838900386ull,   4094160040294753142ull,  2368635764350388458ull,
    15101240511397838657ull, 15584415763303953578ull, 7831857200208015446ull,
    1952643641639729063ull,  4184323302594028609ull,  16795120381104846695ull,
    3541559381538365280ull,  15408472870896842474ull, 5628362450757896366ull,
    16277348886873708846ull, 12437047172652330846ull, 10172715019035948149ull,
    1999700669649752791ull,  6217957085626135027ull,  11220551167830336823ull,
    16478747645632411810ull, 5437280487207382147ull,  11382378739613087836ull,
    15866932785489521505ull, 5502694314775516684ull,  16440179278067648435ull,
    15510104554374162846ull, 15722061259110909195ull, 10760687291786964354ull,
    10736868329920212671ull, 4166148127664495614ull,  14303518358120527892ull,
    9122250801678898571ull,  10028508179936801946ull, 216630713752669403ull,
    10655207865433859491ull, 4041437116174699233ull,  6280982262534375348ull,
    297501356638818866ull,   13976146806363377485ull, 13752396481560145603ull,
    11472199956603637419ull, 16393728429143900496ull, 14752844047515986640ull,
    1524477318846038424ull,  6596889774254235440ull,  1591982099532234960ull,
    8065146456116391065ull,  3964696017750868345ull,  17040425970526664920ull,
    11511165586176539991ull, 3443401252003315103ull,  16314977947073778249ull,
    16860120454903458341ull, 5370503221561340846ull,  15362920279125264094ull,
    2822458124714999779ull,  14575378304387898337ull, 9689406052675046032ull,
    2872149351415175149ull,  13019620945255883050ull, 14929026760148695825ull,
    8503417349692327218ull,  9677798905341573754ull,  828949921821462483ull,
    16110482368362750196ull, 15794218816553655671ull, 14942910774764855088ull,
    12026350906243760195ull, 13610867176871462505ull, 18324536557697872582ull,
    2658962269666727629ull,  327225403251576027ull,   9207535177029277544ull,
    8744129291351887858ull,  6129603385168921503ull,  18385497655031085907ull,
    13024478718952333892ull, 14547683159720717167ull, 5932119629366981711ull,
    325385464632594563ull,   3559879386019806291ull,  6629264948665231298ull,
    14358245326238118181ull, 15662449672706340765ull, 13975503159145803297ull,
    3609534220891499022ull,  4224273587485638227ull,  9274084767162416370ull,
    13156843921244091998ull, 18284750575626858789ull, 14664767920489118779ull,
    11292057742031803221ull, 13919998707305829132ull, 14473305049457001422ull,
    9696877879685767807ull,  1406758246007973837ull,  2429517644459056881ull,
    14361215588101587430ull, 11386164476149757528ull, 10474116023593331839ull,
    2921165656527786564ull,  15604610369733358953ull, 12955027028676000544ull,
    10314281035410779907ull, 3167047178514709947ull,  1088721329408346700ull,
    17930425515478182741ull, 7466411836095405617ull,  15534027454610690575ull,
    10879629128927506091ull, 11502219301371200635ull, 13915106894453889418ull,
    4226784327815861027ull,  12335222183627106346ull, 3648499746356007767ull,
    18441388887898023393ull, 18117929843327093625ull, 4237736098094830438ull,
    14229123019768296655ull, 3930112058127932690ull,  12663879236019645778ull,
    9281161952002617309ull,  4978473890680876319ull,  845759387067546611ull,
    1386164484606776333ull,  8008554770639925512ull,  11159581016793288971ull,
    18065390393740782906ull, 17647985458967631018ull, 9092379465737744314ull,
    2914678236848656327ull,  4376066698447630270ull,  16057186499919087528ull,
    3031333261848790078ull,  2926746602873431597ull,  7931945763526885287ull,
    147649915388326849ull,   15801792398814946230ull, 5265900391686545347ull,
    16173686275871890830ull, 7562781050481886043ull,  5853506575839330404ull,
    14957980734704564792ull, 10944286556353523404ull, 1783009880614150597ull,
    9529762028588888983ull,  822992871011696119ull,   2130074274744257510ull,
    8000279549284809219ull,  3514744284158856431ull,  128770032569293263ull,
    3737367602618100572ull,  16364836605077998543ull, 783266423471782696ull,
    4569418252658970391ull,  11093950688157406886ull, 14888808512267628166ull,
    4217786261273670948ull,  17047486076688645713ull, 14133826721458860485ull,
    17539744882220127106ull, 12394675039129853905ull, 5757634999463277090ull,
    9621947619435861331ull,  1182210208559436772ull,  14603391040490913939ull,
    17481976703660945893ull, 14063388816234683976ull, 2046622692581829572ull,
    8294969799792017441ull,  5293778434844788058ull,  17976364049306763808ull,
    399482430848083948ull,   16495545010129798933ull, 15241340958282367519ull,
    989828753826900814ull,   17616558773874893537ull, 2471817920909589004ull,
    11764082277667899978ull, 9618755269550400950ull,  1240014743757147125ull,
    1887649378641563002ull,  1842982574728131416ull,  13243531042427194002ull,
    7688268125537013927ull,  3080422097287486736ull,  2562894809975407783ull,
    12428984115620094788ull, 1355581933694478148ull,  9895969242586224966ull,
    8628445623963160889ull,  4298916726468199239ull,  12773165416305557280ull,
    5240726258301567487ull,  4975412836403427561ull,  1842172398579595303ull,
    7812151462958058676ull,  17974510987263071769ull, 14980707022065991200ull,
    18294903201142729875ull, 12911672684850242753ull, 8979482998667235743ull,
    16808468362384462073ull, 5981317232108359798ull,  12373702800369335100ull,
    16119707581920094765ull, 2782738549717633602ull,  15454155188515389391ull,
    16495638000603654629ull, 16348757069342790497ull, 7769562861984504567ull,
    17504300515449231559ull, 5557710032938318996ull,  11846125204788401203ull,
    13957316349928882624ull, 2738350683717432043ull,  15738068448047700954ull,
    6224714837294524999ull,  6081930777706411111ull,  11366312928059597928ull,
    4355315799925031482ull,  12393324728734964015ull, 15277140291994338591ull,
    1406052433297386355ull,  15859448364509213398ull, 1672805458341158435ull,
    2926095111610982994ull,  11056431822276774455ull, 12083767323511977430ull,
    3296968762229741153ull,  12312076899982286460ull, 17769284994682227273ull,
    15349428916826953443ull, 1056147296359223910ull,  18305757538706977431ull,
    6214378374180465222ull,  14279648441175008454ull, 17791306410319136644ull,
    956593013486324072ull,   2921235772936241950ull,  10002890515925652606ull,
    10399654693663712506ull, 6446247931049971441ull,  6380465770144534958ull,
    11439178472613251620ull, 10131486500045494660ull, 3692642123868351947ull,
    10972816599561388940ull, 4931112976348785580ull,  8213967169213816566ull,
    15336469859637867841ull,
};