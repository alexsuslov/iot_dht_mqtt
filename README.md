# Simple IoT sensor DHT22 to mqtt

  ┌───────────────────────┐
  │                       │
  │                       │
  │                       │
  │          DHT          │
  │                       │
  │                       │
  │                       │
  │                       │
  └───────────────────────┘
   ┌───┐ ┌───┐ ┌───┐ ┌───┐ 
   │   │ │   │ │   │ │   │ 
   │ 1 │ │ 2 │ │ 3 │ │ 4 │ 
   │   │ │   │ │   │ │   │ 
   └───┘ └───┘ └───┘ └───┘ 
     │     │          │    
     │     │          │    
 ┌───┘     └────┐     │    
 │  ┌────────┐  │     │    
 └┬─│   5k   │─┬┘     │    
  │ └────────┘ │      │    
  └─┐         ┌┘      │    
    │         │       │    
    │         │       │    
┌───────┐ ┌───────┐ ┌───┐  
│gpio 12│ │gpio 14│ │gnd│  
└───────┘ └───────┘ └───┘  


