-- Anub'Arak
UPDATE `creature_template` SET type='0', MovementType='1', InhabitType='4', flags_extra='2' WHERE entry IN ('34606', '34649'); -- '34606', '34649', '0', '0', 'Frost Sphere'
UPDATE `creature_template` SET AIName='NullAI', flags_extra='2' WHERE entry='34862';
UPDATE `creature_template` SET modelid_2='0' WHERE entry IN ('34660', '35652', '35653','35654'); -- '34660', '35652', '35653', '35654', 'Anub''arak'

UPDATE `creature_template` SET ScriptName='mob_anubarak_spike' WHERE entry=34660;
UPDATE `creature_template` SET ScriptName='mob_frost_sphere' WHERE entry=34606;

-- Anubarak Submerge Aura
REPLACE INTO `creature_template_addon` (entry, auras) VALUES ('34564', '65981');
-- Pursuing Spikes "Ground" Aura
REPLACE INTO `creature_template_addon` (entry, auras) VALUES ('34660', '65921');
-- Expose Weakness -- '34607', '34648', '35655', '35656', 'Nerubian Burrower'
REPLACE INTO `creature_template_addon` (entry, auras) VALUES ('34607', '67720');
-- Frostsphere Visual
REPLACE INTO `creature_template_addon` (entry, auras) VALUES ('34606', '67539');

REPLACE INTO `spell_script_target` (entry, type, targetEntry) VALUES ('66339', '1', '34862');
REPLACE INTO `spell_script_target` (entry, type, targetEntry) VALUES ('66332', '1', '34862');
REPLACE INTO `spell_script_target` (entry, type, targetEntry) VALUES ('66170', '1', '34660');
REPLACE INTO `spell_script_target` (entry, type, targetEntry) VALUES ('66170', '1', '35652');
REPLACE INTO `spell_script_target` (entry, type, targetEntry) VALUES ('66170', '1', '35653');
REPLACE INTO `spell_script_target` (entry, type, targetEntry) VALUES ('66170', '1', '35654');
REPLACE INTO `spell_script_target` (entry, type, targetEntry) VALUES ('66181', '1', '34606');
REPLACE INTO `spell_script_target` (entry, type, targetEntry) VALUES ('66181', '1', '34649');

-- Twin Valkyrs
REPLACE INTO `creature_template_addon` (entry, auras) VALUES ('34497', '65766'); -- Fjona light surge
REPLACE INTO `creature_template_addon` (entry, auras) VALUES ('34496', '65768'); -- Eydis dark surge

-- Light twin pact
REPLACE INTO `spell_script_target` (entry, type, targetEntry) VALUES ('65876', '1', '34496');
REPLACE INTO `spell_script_target` (entry, type, targetEntry) VALUES ('67306', '1', '34496');
REPLACE INTO `spell_script_target` (entry, type, targetEntry) VALUES ('67307', '1', '34496');
REPLACE INTO `spell_script_target` (entry, type, targetEntry) VALUES ('67308', '1', '34496');

-- Dark twin pact
REPLACE INTO `spell_script_target` (entry, type, targetEntry) VALUES ('65875', '1', '34497');
REPLACE INTO `spell_script_target` (entry, type, targetEntry) VALUES ('67303', '1', '34497');
REPLACE INTO `spell_script_target` (entry, type, targetEntry) VALUES ('67304', '1', '34497');
REPLACE INTO `spell_script_target` (entry, type, targetEntry) VALUES ('67305', '1', '34497');

-- Hack, should be spellclick! (maybe problems with remove of second aura)
UPDATE `creature_template` SET npcflag='1', ScriptName='mob_essence' WHERE entry IN ('34568', '34567');
