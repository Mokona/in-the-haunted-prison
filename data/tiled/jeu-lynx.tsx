<?xml version="1.0" encoding="UTF-8"?>
<tileset version="1.10" tiledversion="1.10.2" name="jeu-lynx" tilewidth="16" tileheight="16" tilecount="1078" columns="49">
 <image source="monochrome_packed.png" width="784" height="352"/>
 <tile id="5">
  <properties>
   <property name="Obstacle" type="bool" value="false"/>
  </properties>
 </tile>
 <tile id="30">
  <properties>
   <property name="Spawn" value="mc"/>
  </properties>
 </tile>
 <tile id="102">
  <properties>
   <property name="Obstacle" type="bool" value="true"/>
  </properties>
 </tile>
 <tile id="152">
  <properties>
   <property name="Obstacle" type="bool" value="true"/>
  </properties>
 </tile>
 <tile id="171">
  <properties>
   <property name="Inventaire" type="int" value="8"/>
   <property name="Spawn" value="zombie"/>
  </properties>
 </tile>
 <tile id="219">
  <properties>
   <property name="Inventaire" type="int" value="10"/>
   <property name="Object" type="bool" value="true"/>
   <property name="Pickable" type="bool" value="true"/>
   <property name="hidden_treasure" type="bool" value="true"/>
  </properties>
 </tile>
 <tile id="320">
  <properties>
   <property name="Inventaire" type="int" value="9"/>
   <property name="Spawn" value="fantom"/>
  </properties>
 </tile>
 <tile id="323">
  <properties>
   <property name="Inventaire" type="int" value="7"/>
   <property name="Spawn" value="skeleton"/>
  </properties>
 </tile>
 <tile id="327">
  <properties>
   <property name="Damage" type="int" value="2"/>
   <property name="Force" type="int" value="2"/>
   <property name="Inventaire" type="int" value="3"/>
   <property name="Object" type="bool" value="true"/>
   <property name="Pickable" type="bool" value="true"/>
   <property name="Weapon" value="weapon_1"/>
  </properties>
 </tile>
 <tile id="380">
  <properties>
   <property name="Damage" type="int" value="2"/>
   <property name="Force" type="int" value="2"/>
   <property name="Inventaire" type="int" value="4"/>
   <property name="Object" type="bool" value="true"/>
   <property name="Pickable" type="bool" value="true"/>
   <property name="Weapon" value="weapon_2"/>
  </properties>
 </tile>
 <tile id="418">
  <properties>
   <property name="Inventaire" type="int" value="5"/>
   <property name="Spawn" value="bat"/>
  </properties>
 </tile>
 <tile id="423">
  <properties>
   <property name="Inventaire" type="int" value="6"/>
   <property name="Spawn" value="rat"/>
  </properties>
 </tile>
 <tile id="450">
  <properties>
   <property name="Door" type="bool" value="true"/>
   <property name="Opened" type="bool" value="true"/>
   <property name="With_Lever" type="bool" value="true"/>
  </properties>
 </tile>
 <tile id="451">
  <properties>
   <property name="Door" type="bool" value="true"/>
   <property name="Key" value="571"/>
  </properties>
 </tile>
 <tile id="452">
  <properties>
   <property name="Door" type="bool" value="true"/>
   <property name="Key" value="572"/>
  </properties>
 </tile>
 <tile id="493">
  <properties>
   <property name="Activated" type="bool" value="false"/>
   <property name="Lever" type="bool" value="true"/>
   <property name="Object" type="bool" value="true"/>
   <property name="Pickable" type="bool" value="false"/>
  </properties>
 </tile>
 <tile id="494">
  <properties>
   <property name="Activated" type="bool" value="true"/>
   <property name="Lever" type="bool" value="true"/>
   <property name="Object" type="bool" value="true"/>
   <property name="Pickable" type="bool" value="false"/>
  </properties>
 </tile>
 <tile id="501">
  <properties>
   <property name="Door" type="bool" value="true"/>
   <property name="Key" value="573"/>
  </properties>
 </tile>
 <tile id="542">
  <properties>
   <property name="Obstacle" type="bool" value="true"/>
  </properties>
 </tile>
 <tile id="550">
  <properties>
   <property name="Door" type="bool" value="true"/>
   <property name="Opened" type="bool" value="false"/>
   <property name="With_Lever" type="bool" value="true"/>
  </properties>
 </tile>
 <tile id="551">
  <properties>
   <property name="Door" type="bool" value="true"/>
   <property name="Opened" type="bool" value="true"/>
  </properties>
 </tile>
 <tile id="571">
  <properties>
   <property name="Inventaire" type="int" value="0"/>
   <property name="Key" type="bool" value="true"/>
   <property name="Object" type="bool" value="true"/>
   <property name="Pickable" type="bool" value="true"/>
  </properties>
 </tile>
 <tile id="572">
  <properties>
   <property name="Inventaire" type="int" value="1"/>
   <property name="Key" type="bool" value="true"/>
   <property name="Object" type="bool" value="true"/>
   <property name="Pickable" type="bool" value="true"/>
  </properties>
 </tile>
 <tile id="573">
  <properties>
   <property name="Inventaire" type="int" value="2"/>
   <property name="Key" type="bool" value="true"/>
   <property name="Object" type="bool" value="true"/>
   <property name="Pickable" type="bool" value="true"/>
  </properties>
 </tile>
 <tile id="643">
  <properties>
   <property name="Obstacle" type="bool" value="true"/>
  </properties>
 </tile>
 <tile id="645">
  <properties>
   <property name="Obstacle" type="bool" value="true"/>
  </properties>
 </tile>
 <tile id="669">
  <properties>
   <property name="HP" type="int" value="5"/>
   <property name="Object" type="bool" value="true"/>
   <property name="Pickable" type="bool" value="true"/>
  </properties>
 </tile>
 <tile id="691">
  <properties>
   <property name="Inventaire" type="int" value="255"/>
   <property name="Object" type="bool" value="true"/>
   <property name="Pickable" type="bool" value="false"/>
   <property name="cauldron" type="bool" value="true"/>
  </properties>
 </tile>
 <tile id="738">
  <properties>
   <property name="Obstacle" type="bool" value="true"/>
  </properties>
 </tile>
 <tile id="741">
  <properties>
   <property name="Breakable" type="int" value="2"/>
   <property name="Inventaire" type="int" value="255"/>
   <property name="Obstacle" type="bool" value="true"/>
   <property name="Resistance" type="int" value="10"/>
   <property name="Spawn" value="wall"/>
  </properties>
 </tile>
 <tile id="742">
  <properties>
   <property name="Obstacle" type="bool" value="true"/>
  </properties>
 </tile>
 <tile id="789">
  <properties>
   <property name="Obstacle" type="bool" value="true"/>
  </properties>
 </tile>
 <tile id="843">
  <properties>
   <property name="Obstacle" type="bool" value="true"/>
  </properties>
 </tile>
</tileset>
