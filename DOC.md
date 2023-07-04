# Communication protocol

## message sent by ARDUINO

```
{
  mid: string;
  messageType: string;
  payload: {
    id: string;
    type: string;
    name: {
      name: string;
    },
    on: boolean;
  }
}
```

### Attributes

- type (devices types)

  ```
  - action.devices.types.OUTLET
  ```

## message sent by server

```
{
  mid: string;
  payload: {
    messageType: string;
    command: {
      on: boolean;
    }
  }
}
```

### Attributes

- messageType
  
  ```
  - QUERY
  - EXECUTE
  ```
