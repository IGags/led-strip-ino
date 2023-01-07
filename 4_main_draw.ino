void drawEffect(){
  switch(settings.mode){
    case 0xFFA25D:empty();break;
    case 0xFF6897:solid();break;
    case 0xFF9867:pulse();break;
    case 0xFFB04F:dots();break;
    case 0xFF30CF:coloredDots();break;
    case 0xFF18E7:rainbow();break;
    case 0xFF7A85:slightFill();break;
    case 0xFF10EF:cyclone();break;
    case 0xFF38C7:fire();break;
    case 0xFF5AA5:fireTick();break;
    case 0xFF42BD:sides();break;
    case 0xFF4AB5:fill();break;
    case 0xFF52AD:cells();break;
  }
}