{
  "annotations": {
    "list": [
      {
        "builtIn": 1,
        "datasource": "-- Grafana --",
        "enable": true,
        "hide": true,
        "iconColor": "rgba(0, 211, 255, 1)",
        "name": "Annotations & Alerts",
        "type": "dashboard"
      }
    ]
  },
  "editable": true,
  "gnetId": null,
  "graphTooltip": 2,
  "id": 2,
  "links": [],
  "panels": [
    {
      "cards": {
        "cardHSpacing": 2,
        "cardMinWidth": 5,
        "cardRound": null,
        "cardVSpacing": 2
      },
      "color": {
        "cardColor": "#b4ff00",
        "colorScale": "sqrt",
        "colorScheme": "interpolateGnYlRd",
        "defaultColor": "#757575",
        "exponent": 0.5,
        "mode": "discrete",
        "thresholds": [
          {
            "color": "#FF9830",
            "tooltip": "Antena 3",
            "value": "3"
          },
          {
            "color": "#C4162A",
            "tooltip": "Cuatro",
            "value": "4"
          },
          {
            "color": "#C0D8FF",
            "tooltip": "Telecinco",
            "value": "5"
          },
          {
            "color": "#73BF69",
            "tooltip": "FOX",
            "value": "11"
          },
          {
            "color": "#B877D9",
            "tooltip": "TNT",
            "value": "9"
          },
          {
            "color": "#1F60C4",
            "tooltip": "TNT HD",
            "value": "20"
          },
          {
            "color": "#FF7383",
            "tooltip": "Discovery Channel",
            "value": "18"
          },
          {
            "color": "#DEB6F2",
            "tooltip": "#0 HD",
            "value": "21"
          },
          {
            "color": "#FFEE52",
            "tooltip": "Neox",
            "value": "15"
          }
        ]
      },
      "data": {
        "decimals": null,
        "unitFormat": "short"
      },
      "datasource": "MySQL",
      "description": "Muestra los canales que han ido viendo los usuarios. La gráfica utiliza los tiempos de report de IGMP.",
      "gridPos": {
        "h": 8,
        "w": 12,
        "x": 0,
        "y": 0
      },
      "highlightCards": true,
      "id": 22,
      "legend": {
        "show": true
      },
      "nullPointMode": "as empty",
      "targets": [
        {
          "format": "time_series",
          "group": [],
          "metricColumn": "none",
          "rawQuery": true,
          "rawSql": "SELECT\n  Tiempo AS \"time\",\n  Usuario AS metric,\n  id\nFROM Igmp INNER JOIN Programas ON Igmp.Ip = Programas.Ip\nWHERE\n  $__unixEpochFilter(Tiempo)\nORDER BY Tiempo",
          "refId": "A",
          "select": [
            [
              {
                "params": [
                  "Tiempo"
                ],
                "type": "column"
              }
            ]
          ],
          "table": "Igmp",
          "timeColumn": "Tiempo",
          "timeColumnType": "double",
          "where": [
            {
              "name": "$__unixEpochFilter",
              "params": [],
              "type": "macro"
            }
          ]
        }
      ],
      "timeFrom": null,
      "timeShift": null,
      "title": "Canales visualizados",
      "tooltip": {
        "show": true
      },
      "type": "flant-statusmap-panel",
      "useMax": true,
      "xAxis": {
        "labelFormat": "%a %m/%d",
        "minBucketWidthToShowWeekends": 4,
        "show": true,
        "showCrosshair": true,
        "showWeekends": true
      },
      "yAxis": {
        "show": true,
        "showCrosshair": false
      },
      "yAxisSort": "metrics"
    },
    {
      "cacheTimeout": null,
      "colorBackground": true,
      "colorValue": false,
      "colors": [
        "#F2495C",
        "#FF9830",
        "#73BF69"
      ],
      "datasource": "MySQL",
      "decimals": 2,
      "description": "Valor medio de MOS",
      "format": "none",
      "gauge": {
        "maxValue": 100,
        "minValue": 0,
        "show": false,
        "thresholdLabels": false,
        "thresholdMarkers": true
      },
      "gridPos": {
        "h": 4,
        "w": 4,
        "x": 12,
        "y": 0
      },
      "id": 28,
      "interval": "10s",
      "links": [],
      "mappingType": 1,
      "mappingTypes": [
        {
          "name": "value to text",
          "value": 1
        },
        {
          "name": "range to text",
          "value": 2
        }
      ],
      "maxDataPoints": 100,
      "nullPointMode": "connected",
      "nullText": null,
      "postfix": "",
      "postfixFontSize": "50%",
      "prefix": "",
      "prefixFontSize": "50%",
      "rangeMaps": [
        {
          "from": "null",
          "text": "N/A",
          "to": "null"
        }
      ],
      "sparkline": {
        "fillColor": "rgba(31, 118, 189, 0.18)",
        "full": false,
        "lineColor": "rgb(31, 120, 193)",
        "show": false,
        "ymax": null,
        "ymin": null
      },
      "tableColumn": "",
      "targets": [
        {
          "format": "time_series",
          "group": [],
          "metricColumn": "none",
          "rawQuery": true,
          "rawSql": "SELECT\n  Tiempo AS \"time\",\n  Mos\nFROM Canales\nWHERE\n  $__unixEpochFilter(Tiempo) AND Tipo=0\nORDER BY Tiempo",
          "refId": "A",
          "select": [
            [
              {
                "params": [
                  "Mos"
                ],
                "type": "column"
              }
            ]
          ],
          "table": "Canales",
          "timeColumn": "Tiempo",
          "timeColumnType": "double",
          "where": [
            {
              "name": "$__unixEpochFilter",
              "params": [],
              "type": "macro"
            }
          ]
        },
        {
          "format": "time_series",
          "group": [],
          "hide": true,
          "metricColumn": "none",
          "rawQuery": true,
          "rawSql": "SELECT\n  Tiempo AS \"time\",\n  Mos\nFROM Canales\nWHERE\n  $__unixEpochFilter(Tiempo) AND Tipo=1\nORDER BY Tiempo",
          "refId": "B",
          "select": [
            [
              {
                "params": [
                  "Mos"
                ],
                "type": "column"
              }
            ]
          ],
          "table": "Canales",
          "timeColumn": "Tiempo",
          "timeColumnType": "double",
          "where": [
            {
              "name": "$__unixEpochFilter",
              "params": [],
              "type": "macro"
            }
          ]
        }
      ],
      "thresholds": "3,4",
      "timeFrom": null,
      "timeShift": null,
      "title": "MOS medio",
      "type": "singlestat",
      "valueFontSize": "80%",
      "valueMaps": [
        {
          "op": "=",
          "text": "N/A",
          "value": "null"
        }
      ],
      "valueName": "avg"
    },
    {
      "cacheTimeout": null,
      "colorBackground": true,
      "colorValue": false,
      "colors": [
        "#73BF69",
        "#FF9830",
        "#F2495C"
      ],
      "datasource": "MySQL",
      "description": "Media de porcentaje de perdidas",
      "format": "percent",
      "gauge": {
        "maxValue": 100,
        "minValue": 0,
        "show": false,
        "thresholdLabels": false,
        "thresholdMarkers": true
      },
      "gridPos": {
        "h": 4,
        "w": 4,
        "x": 16,
        "y": 0
      },
      "id": 26,
      "interval": "",
      "links": [],
      "mappingType": 1,
      "mappingTypes": [
        {
          "name": "value to text",
          "value": 1
        },
        {
          "name": "range to text",
          "value": 2
        }
      ],
      "maxDataPoints": 100,
      "nullPointMode": "connected",
      "nullText": null,
      "postfix": "",
      "postfixFontSize": "50%",
      "prefix": "",
      "prefixFontSize": "50%",
      "rangeMaps": [
        {
          "from": "null",
          "text": "N/A",
          "to": "null"
        }
      ],
      "sparkline": {
        "fillColor": "rgba(31, 118, 189, 0.18)",
        "full": false,
        "lineColor": "rgb(31, 120, 193)",
        "show": false,
        "ymax": null,
        "ymin": null
      },
      "tableColumn": "",
      "targets": [
        {
          "format": "time_series",
          "group": [],
          "metricColumn": "none",
          "rawQuery": true,
          "rawSql": "SELECT\n  Tiempo AS \"time\",\n  PorPerDif\nFROM Canales\nWHERE\n  $__unixEpochFilter(Tiempo) AND Tipo=0\nORDER BY Tiempo",
          "refId": "A",
          "select": [
            [
              {
                "params": [
                  "PorPerDif"
                ],
                "type": "column"
              }
            ]
          ],
          "table": "Canales",
          "timeColumn": "Tiempo",
          "timeColumnType": "double",
          "where": [
            {
              "name": "$__unixEpochFilter",
              "params": [],
              "type": "macro"
            }
          ]
        },
        {
          "format": "time_series",
          "group": [],
          "hide": true,
          "metricColumn": "none",
          "rawQuery": true,
          "rawSql": "SELECT\n  Tiempo AS \"time\",\n  PorPerDif\nFROM Canales\nWHERE\n  $__unixEpochFilter(Tiempo) AND Tipo=1\nORDER BY Tiempo",
          "refId": "B",
          "select": [
            [
              {
                "params": [
                  "PorPerDif"
                ],
                "type": "column"
              }
            ]
          ],
          "table": "Canales",
          "timeColumn": "Tiempo",
          "timeColumnType": "double",
          "where": [
            {
              "name": "$__unixEpochFilter",
              "params": [],
              "type": "macro"
            }
          ]
        }
      ],
      "thresholds": "0.1,1",
      "timeFrom": null,
      "timeShift": null,
      "title": "% de perdidas medio",
      "type": "singlestat",
      "valueFontSize": "80%",
      "valueMaps": [
        {
          "op": "=",
          "text": "N/A",
          "value": "null"
        }
      ],
      "valueName": "avg"
    },
    {
      "cacheTimeout": null,
      "colorBackground": true,
      "colorValue": false,
      "colors": [
        "#73BF69",
        "#FF9830",
        "#F2495C"
      ],
      "datasource": "MySQL",
      "description": "Valor medio de Jitter",
      "format": "µs",
      "gauge": {
        "maxValue": 100,
        "minValue": 0,
        "show": false,
        "thresholdLabels": false,
        "thresholdMarkers": true
      },
      "gridPos": {
        "h": 4,
        "w": 4,
        "x": 20,
        "y": 0
      },
      "id": 32,
      "interval": null,
      "links": [],
      "mappingType": 1,
      "mappingTypes": [
        {
          "name": "value to text",
          "value": 1
        },
        {
          "name": "range to text",
          "value": 2
        }
      ],
      "maxDataPoints": 100,
      "nullPointMode": "connected",
      "nullText": null,
      "postfix": "",
      "postfixFontSize": "50%",
      "prefix": "",
      "prefixFontSize": "50%",
      "rangeMaps": [
        {
          "from": "null",
          "text": "N/A",
          "to": "null"
        }
      ],
      "sparkline": {
        "fillColor": "rgba(31, 118, 189, 0.18)",
        "full": false,
        "lineColor": "rgb(31, 120, 193)",
        "show": false,
        "ymax": null,
        "ymin": null
      },
      "tableColumn": "",
      "targets": [
        {
          "format": "time_series",
          "group": [],
          "metricColumn": "none",
          "rawQuery": true,
          "rawSql": "SELECT\n  Tiempo AS \"time\",\n  Jitter\nFROM Canales\nWHERE\n  $__unixEpochFilter(Tiempo) AND Tipo=0\nORDER BY Tiempo",
          "refId": "A",
          "select": [
            [
              {
                "params": [
                  "Jitter"
                ],
                "type": "column"
              }
            ]
          ],
          "table": "Canales",
          "timeColumn": "Tiempo",
          "timeColumnType": "double",
          "where": [
            {
              "name": "$__unixEpochFilter",
              "params": [],
              "type": "macro"
            }
          ]
        },
        {
          "format": "time_series",
          "group": [],
          "hide": true,
          "metricColumn": "none",
          "rawQuery": true,
          "rawSql": "SELECT\n  Tiempo AS \"time\",\n  Jitter\nFROM Canales\nWHERE\n  $__unixEpochFilter(Tiempo) AND Tipo=1\nORDER BY Tiempo",
          "refId": "B",
          "select": [
            [
              {
                "params": [
                  "Jitter"
                ],
                "type": "column"
              }
            ]
          ],
          "table": "Canales",
          "timeColumn": "Tiempo",
          "timeColumnType": "double",
          "where": [
            {
              "name": "$__unixEpochFilter",
              "params": [],
              "type": "macro"
            }
          ]
        }
      ],
      "thresholds": "100000,30000",
      "timeFrom": null,
      "timeShift": null,
      "title": "JITTER medio",
      "type": "singlestat",
      "valueFontSize": "80%",
      "valueMaps": [
        {
          "op": "=",
          "text": "N/A",
          "value": "null"
        }
      ],
      "valueName": "avg"
    },
    {
      "cacheTimeout": null,
      "colorBackground": false,
      "colorPrefix": false,
      "colorValue": false,
      "colors": [
        "rgb(0, 0, 0)",
        "rgba(237, 129, 40, 0.89)",
        "#d44a3a"
      ],
      "datasource": "MySQL",
      "description": "Valor medio de anomalías  IGMP",
      "format": "none",
      "gauge": {
        "maxValue": 100,
        "minValue": 0,
        "show": false,
        "thresholdLabels": false,
        "thresholdMarkers": true
      },
      "gridPos": {
        "h": 4,
        "w": 4,
        "x": 12,
        "y": 4
      },
      "id": 24,
      "interval": null,
      "links": [],
      "mappingType": 1,
      "mappingTypes": [
        {
          "name": "value to text",
          "value": 1
        },
        {
          "name": "range to text",
          "value": 2
        }
      ],
      "maxDataPoints": 100,
      "nullPointMode": "connected",
      "nullText": null,
      "postfix": "",
      "postfixFontSize": "50%",
      "prefix": "",
      "prefixFontSize": "50%",
      "rangeMaps": [
        {
          "from": "null",
          "text": "N/A",
          "to": "null"
        }
      ],
      "sparkline": {
        "fillColor": "rgba(31, 118, 189, 0.18)",
        "full": false,
        "lineColor": "rgb(31, 120, 193)",
        "show": false,
        "ymax": null,
        "ymin": null
      },
      "tableColumn": "",
      "targets": [
        {
          "format": "time_series",
          "group": [],
          "metricColumn": "none",
          "rawQuery": true,
          "rawSql": "SELECT\n  Tiempo AS \"time\",\n  NumErrDif\nFROM Canales\nWHERE\n  $__unixEpochFilter(Tiempo) AND Tipo=0\nORDER BY Tiempo",
          "refId": "A",
          "select": [
            [
              {
                "params": [
                  "NumErrDif"
                ],
                "type": "column"
              }
            ]
          ],
          "table": "Canales",
          "timeColumn": "Tiempo",
          "timeColumnType": "double",
          "where": [
            {
              "name": "$__unixEpochFilter",
              "params": [],
              "type": "macro"
            }
          ]
        },
        {
          "format": "time_series",
          "group": [],
          "hide": true,
          "metricColumn": "none",
          "rawQuery": true,
          "rawSql": "SELECT\n  Tiempo AS \"time\",\n  NumErrDif\nFROM Canales\nWHERE\n  $__unixEpochFilter(Tiempo) AND Tipo=1\nORDER BY Tiempo",
          "refId": "B",
          "select": [
            [
              {
                "params": [
                  "NumErrDif"
                ],
                "type": "column"
              }
            ]
          ],
          "table": "Canales",
          "timeColumn": "Tiempo",
          "timeColumnType": "double",
          "where": [
            {
              "name": "$__unixEpochFilter",
              "params": [],
              "type": "macro"
            }
          ]
        }
      ],
      "thresholds": "",
      "timeFrom": null,
      "timeShift": null,
      "title": "Media de anomalias IGMP",
      "type": "singlestat",
      "valueFontSize": "80%",
      "valueMaps": [
        {
          "op": "=",
          "text": "N/A",
          "value": "null"
        }
      ],
      "valueName": "avg"
    },
    {
      "cacheTimeout": null,
      "colorBackground": false,
      "colorValue": false,
      "colors": [
        "#1F60C4",
        "rgba(237, 129, 40, 0.89)",
        "#d44a3a"
      ],
      "datasource": "MySQL",
      "description": "Valor medio de throughput",
      "format": "Mbits",
      "gauge": {
        "maxValue": 100,
        "minValue": 0,
        "show": false,
        "thresholdLabels": false,
        "thresholdMarkers": true
      },
      "gridPos": {
        "h": 4,
        "w": 4,
        "x": 16,
        "y": 4
      },
      "id": 30,
      "interval": null,
      "links": [],
      "mappingType": 1,
      "mappingTypes": [
        {
          "name": "value to text",
          "value": 1
        },
        {
          "name": "range to text",
          "value": 2
        }
      ],
      "maxDataPoints": 100,
      "nullPointMode": "connected",
      "nullText": null,
      "postfix": "",
      "postfixFontSize": "50%",
      "prefix": "",
      "prefixFontSize": "50%",
      "rangeMaps": [
        {
          "from": "null",
          "text": "N/A",
          "to": "null"
        }
      ],
      "sparkline": {
        "fillColor": "rgba(31, 118, 189, 0.18)",
        "full": false,
        "lineColor": "rgb(31, 120, 193)",
        "show": false,
        "ymax": null,
        "ymin": null
      },
      "tableColumn": "",
      "targets": [
        {
          "format": "time_series",
          "group": [],
          "metricColumn": "none",
          "rawQuery": true,
          "rawSql": "SELECT\n  Tiempo AS \"time\",\n  Throughput\nFROM Canales\nWHERE\n  $__unixEpochFilter(Tiempo) AND Tipo=0\nORDER BY Tiempo",
          "refId": "A",
          "select": [
            [
              {
                "params": [
                  "BytesDiff"
                ],
                "type": "column"
              }
            ]
          ],
          "table": "Canales",
          "timeColumn": "Tiempo",
          "timeColumnType": "double",
          "where": [
            {
              "name": "$__unixEpochFilter",
              "params": [],
              "type": "macro"
            }
          ]
        },
        {
          "format": "time_series",
          "group": [],
          "hide": true,
          "metricColumn": "none",
          "rawQuery": true,
          "rawSql": "SELECT\n  Tiempo AS \"time\",\n  Throughput\nFROM Canales\nWHERE\n  $__unixEpochFilter(Tiempo) AND Tipo=1\nORDER BY Tiempo",
          "refId": "B",
          "select": [
            [
              {
                "params": [
                  "BytesDiff"
                ],
                "type": "column"
              }
            ]
          ],
          "table": "Canales",
          "timeColumn": "Tiempo",
          "timeColumnType": "double",
          "where": [
            {
              "name": "$__unixEpochFilter",
              "params": [],
              "type": "macro"
            }
          ]
        }
      ],
      "thresholds": "",
      "timeFrom": null,
      "timeShift": null,
      "title": "THROUGHPUT medio",
      "type": "singlestat",
      "valueFontSize": "80%",
      "valueMaps": [
        {
          "op": "=",
          "text": "N/A",
          "value": "null"
        }
      ],
      "valueName": "avg"
    },
    {
      "cacheTimeout": null,
      "colorBackground": false,
      "colorValue": false,
      "colors": [
        "#1F60C4",
        "rgba(237, 129, 40, 0.89)",
        "#d44a3a"
      ],
      "datasource": "MySQL",
      "description": "Valor medio de paquetes recibidos de otro tráfico",
      "format": "none",
      "gauge": {
        "maxValue": 100,
        "minValue": 0,
        "show": false,
        "thresholdLabels": false,
        "thresholdMarkers": true
      },
      "gridPos": {
        "h": 4,
        "w": 4,
        "x": 20,
        "y": 4
      },
      "id": 34,
      "interval": null,
      "links": [],
      "mappingType": 1,
      "mappingTypes": [
        {
          "name": "value to text",
          "value": 1
        },
        {
          "name": "range to text",
          "value": 2
        }
      ],
      "maxDataPoints": 100,
      "nullPointMode": "connected",
      "nullText": null,
      "postfix": " Packs",
      "postfixFontSize": "50%",
      "prefix": "",
      "prefixFontSize": "50%",
      "rangeMaps": [
        {
          "from": "null",
          "text": "N/A",
          "to": "null"
        }
      ],
      "sparkline": {
        "fillColor": "rgba(31, 118, 189, 0.18)",
        "full": false,
        "lineColor": "rgb(31, 120, 193)",
        "show": false,
        "ymax": null,
        "ymin": null
      },
      "tableColumn": "",
      "targets": [
        {
          "format": "time_series",
          "group": [],
          "metricColumn": "none",
          "rawQuery": false,
          "rawSql": "SELECT\n  Tiempo AS \"time\",\n  Total\nFROM Ruido\nWHERE\n  $__unixEpochFilter(Tiempo)\nORDER BY Tiempo",
          "refId": "A",
          "select": [
            [
              {
                "params": [
                  "Total"
                ],
                "type": "column"
              }
            ]
          ],
          "table": "Ruido",
          "timeColumn": "Tiempo",
          "timeColumnType": "double",
          "where": [
            {
              "name": "$__unixEpochFilter",
              "params": [],
              "type": "macro"
            }
          ]
        }
      ],
      "thresholds": "",
      "timeFrom": null,
      "timeShift": null,
      "title": "Media de paquetes UDP recibidos de otro tráfico",
      "type": "singlestat",
      "valueFontSize": "80%",
      "valueMaps": [
        {
          "op": "=",
          "text": "N/A",
          "value": "null"
        }
      ],
      "valueName": "avg"
    },
    {
      "aliasColors": {},
      "bars": false,
      "dashLength": 10,
      "dashes": false,
      "datasource": "MySQL",
      "description": "MOS de la transmisión cada 10 segundos",
      "fill": 1,
      "fillGradient": 0,
      "gridPos": {
        "h": 8,
        "w": 12,
        "x": 0,
        "y": 8
      },
      "hiddenSeries": false,
      "id": 14,
      "legend": {
        "avg": false,
        "current": false,
        "max": false,
        "min": false,
        "show": true,
        "total": false,
        "values": false
      },
      "lines": true,
      "linewidth": 1,
      "nullPointMode": "null",
      "options": {
        "dataLinks": []
      },
      "percentage": false,
      "pointradius": 2,
      "points": false,
      "renderer": "flot",
      "seriesOverrides": [],
      "spaceLength": 10,
      "stack": false,
      "steppedLine": false,
      "targets": [
        {
          "format": "time_series",
          "group": [],
          "hide": false,
          "metricColumn": "Ip",
          "rawQuery": true,
          "rawSql": "SELECT\n  Tiempo AS \"time\",\n  Title AS metric,\n  Mos\nFROM Canales INNER JOIN Programas ON Canales.Ip = Programas.Ip\nWHERE\n  Tipo = 0\nORDER BY Tiempo, Canales.id",
          "refId": "A",
          "select": [
            [
              {
                "params": [
                  "Moss"
                ],
                "type": "column"
              }
            ]
          ],
          "table": "Canales",
          "timeColumn": "Tiempo",
          "timeColumnType": "double",
          "where": [
            {
              "name": "$__unixEpochFilter",
              "params": [],
              "type": "macro"
            },
            {
              "datatype": "int",
              "name": "",
              "params": [
                "Tipo",
                "=",
                "0"
              ],
              "type": "expression"
            }
          ]
        },
        {
          "format": "time_series",
          "group": [],
          "hide": true,
          "metricColumn": "Ip",
          "rawQuery": true,
          "rawSql": "SELECT\n  Tiempo AS \"time\",\n  Ip AS metric,\n  Mos\nFROM Canales\nWHERE\n  Tipo = 1\nORDER BY Tiempo, id",
          "refId": "B",
          "select": [
            [
              {
                "params": [
                  "Mos"
                ],
                "type": "column"
              }
            ]
          ],
          "table": "Canales",
          "timeColumn": "Tiempo",
          "timeColumnType": "double",
          "where": [
            {
              "name": "$__unixEpochFilter",
              "params": [],
              "type": "macro"
            },
            {
              "datatype": "int",
              "name": "",
              "params": [
                "Tipo",
                "=",
                "1"
              ],
              "type": "expression"
            }
          ]
        }
      ],
      "thresholds": [],
      "timeFrom": null,
      "timeRegions": [],
      "timeShift": null,
      "title": "MOS",
      "tooltip": {
        "shared": true,
        "sort": 0,
        "value_type": "individual"
      },
      "type": "graph",
      "xaxis": {
        "buckets": null,
        "mode": "time",
        "name": null,
        "show": true,
        "values": []
      },
      "yaxes": [
        {
          "format": "short",
          "label": "MOS",
          "logBase": 1,
          "max": null,
          "min": null,
          "show": true
        },
        {
          "format": "short",
          "label": null,
          "logBase": 1,
          "max": null,
          "min": null,
          "show": true
        }
      ],
      "yaxis": {
        "align": false,
        "alignLevel": null
      }
    },
    {
      "aliasColors": {},
      "bars": false,
      "dashLength": 10,
      "dashes": false,
      "datasource": "MySQL",
      "description": "Porcentaje de pérdidas acumulativo y en intervalos de 10 segundos",
      "fill": 1,
      "fillGradient": 0,
      "gridPos": {
        "h": 8,
        "w": 12,
        "x": 12,
        "y": 8
      },
      "hiddenSeries": false,
      "id": 10,
      "legend": {
        "avg": false,
        "current": false,
        "max": false,
        "min": false,
        "show": true,
        "total": false,
        "values": false
      },
      "lines": true,
      "linewidth": 1,
      "nullPointMode": "null",
      "options": {
        "dataLinks": []
      },
      "percentage": false,
      "pointradius": 2,
      "points": false,
      "renderer": "flot",
      "seriesOverrides": [],
      "spaceLength": 10,
      "stack": false,
      "steppedLine": false,
      "targets": [
        {
          "format": "time_series",
          "group": [],
          "hide": false,
          "metricColumn": "Ip",
          "rawQuery": true,
          "rawSql": "SELECT\n  Tiempo AS \"time\",\n  Title AS metric,\n  PorPerDif\nFROM Canales INNER JOIN Programas ON Canales.Ip = Programas.Ip\nWHERE\n  Tipo = 0\nORDER BY Tiempo,Canales.id",
          "refId": "B",
          "select": [
            [
              {
                "params": [
                  "PorPerDif"
                ],
                "type": "column"
              }
            ]
          ],
          "table": "Canales",
          "timeColumn": "Tiempo",
          "timeColumnType": "double",
          "where": [
            {
              "name": "$__unixEpochFilter",
              "params": [],
              "type": "macro"
            },
            {
              "datatype": "int",
              "name": "",
              "params": [
                "Tipo",
                "=",
                "0"
              ],
              "type": "expression"
            }
          ]
        },
        {
          "format": "time_series",
          "group": [],
          "hide": true,
          "metricColumn": "Ip",
          "rawQuery": true,
          "rawSql": "SELECT\n  Tiempo AS \"time\",\n  Ip AS metric,\n  PorPerDif\nFROM Canales\nWHERE\n  Tipo = 1\nORDER BY Tiempo, id",
          "refId": "A",
          "select": [
            [
              {
                "params": [
                  "PorPerDif"
                ],
                "type": "column"
              }
            ]
          ],
          "table": "Canales",
          "timeColumn": "Tiempo",
          "timeColumnType": "double",
          "where": [
            {
              "name": "$__unixEpochFilter",
              "params": [],
              "type": "macro"
            },
            {
              "datatype": "int",
              "name": "",
              "params": [
                "Tipo",
                "=",
                "1"
              ],
              "type": "expression"
            }
          ]
        }
      ],
      "thresholds": [],
      "timeFrom": null,
      "timeRegions": [],
      "timeShift": null,
      "title": "Porcentaje de perdidas en 10 segundos",
      "tooltip": {
        "shared": true,
        "sort": 0,
        "value_type": "individual"
      },
      "type": "graph",
      "xaxis": {
        "buckets": null,
        "mode": "time",
        "name": null,
        "show": true,
        "values": []
      },
      "yaxes": [
        {
          "format": "percent",
          "label": "Perdidas",
          "logBase": 1,
          "max": null,
          "min": null,
          "show": true
        },
        {
          "format": "short",
          "label": null,
          "logBase": 1,
          "max": null,
          "min": null,
          "show": false
        }
      ],
      "yaxis": {
        "align": false,
        "alignLevel": null
      }
    },
    {
      "aliasColors": {},
      "bars": false,
      "dashLength": 10,
      "dashes": false,
      "datasource": "MySQL",
      "description": "Variación del Jitter en intervalos de 10 segundos",
      "fill": 1,
      "fillGradient": 0,
      "gridPos": {
        "h": 8,
        "w": 12,
        "x": 0,
        "y": 16
      },
      "hiddenSeries": false,
      "id": 4,
      "legend": {
        "alignAsTable": false,
        "avg": false,
        "current": false,
        "max": false,
        "min": false,
        "rightSide": false,
        "show": true,
        "total": false,
        "values": false
      },
      "lines": true,
      "linewidth": 1,
      "nullPointMode": "null",
      "options": {
        "dataLinks": []
      },
      "percentage": false,
      "pointradius": 2,
      "points": false,
      "renderer": "flot",
      "seriesOverrides": [],
      "spaceLength": 10,
      "stack": false,
      "steppedLine": false,
      "targets": [
        {
          "format": "time_series",
          "group": [],
          "metricColumn": "Ip",
          "rawQuery": true,
          "rawSql": "SELECT\n  Tiempo AS \"time\",\n  Title AS metric,\n  Jitter\nFROM Canales INNER JOIN Programas ON Canales.Ip = Programas.Ip\nWHERE\n  Tipo = 0\nORDER BY Tiempo,Canales.id",
          "refId": "A",
          "select": [
            [
              {
                "params": [
                  "Jitter"
                ],
                "type": "column"
              }
            ]
          ],
          "table": "Canales",
          "timeColumn": "Tiempo",
          "timeColumnType": "double",
          "where": [
            {
              "name": "$__unixEpochFilter",
              "params": [],
              "type": "macro"
            },
            {
              "datatype": "int",
              "name": "",
              "params": [
                "Tipo",
                "=",
                "0"
              ],
              "type": "expression"
            }
          ]
        },
        {
          "format": "time_series",
          "group": [],
          "hide": true,
          "metricColumn": "Ip",
          "rawQuery": true,
          "rawSql": "SELECT\n  Tiempo AS \"time\",\n  Ip AS metric,\n  Jitter\nFROM Canales\nWHERE\n  Tipo = 1\nORDER BY Tiempo,id",
          "refId": "B",
          "select": [
            [
              {
                "params": [
                  "Jitter"
                ],
                "type": "column"
              }
            ]
          ],
          "table": "Canales",
          "timeColumn": "Tiempo",
          "timeColumnType": "double",
          "where": [
            {
              "name": "$__unixEpochFilter",
              "params": [],
              "type": "macro"
            },
            {
              "datatype": "int",
              "name": "",
              "params": [
                "Tipo",
                "=",
                "1"
              ],
              "type": "expression"
            }
          ]
        }
      ],
      "thresholds": [],
      "timeFrom": null,
      "timeRegions": [],
      "timeShift": null,
      "title": "Jitter en 10 segundos",
      "tooltip": {
        "shared": true,
        "sort": 0,
        "value_type": "individual"
      },
      "type": "graph",
      "xaxis": {
        "buckets": null,
        "mode": "time",
        "name": null,
        "show": true,
        "values": []
      },
      "yaxes": [
        {
          "format": "none",
          "label": "Microsegundos",
          "logBase": 1,
          "max": null,
          "min": null,
          "show": true
        },
        {
          "format": "short",
          "label": null,
          "logBase": 1,
          "max": null,
          "min": null,
          "show": false
        }
      ],
      "yaxis": {
        "align": false,
        "alignLevel": null
      }
    },
    {
      "aliasColors": {},
      "bars": false,
      "dashLength": 10,
      "dashes": false,
      "datasource": "MySQL",
      "description": "Anomalías de IGMP acumulativas y en intervalos de 10 segundos",
      "fill": 1,
      "fillGradient": 0,
      "gridPos": {
        "h": 8,
        "w": 12,
        "x": 12,
        "y": 16
      },
      "hiddenSeries": false,
      "id": 6,
      "legend": {
        "avg": false,
        "current": false,
        "max": false,
        "min": false,
        "show": true,
        "total": false,
        "values": false
      },
      "lines": true,
      "linewidth": 1,
      "nullPointMode": "null",
      "options": {
        "dataLinks": []
      },
      "percentage": false,
      "pointradius": 2,
      "points": false,
      "renderer": "flot",
      "seriesOverrides": [],
      "spaceLength": 10,
      "stack": false,
      "steppedLine": false,
      "targets": [
        {
          "format": "time_series",
          "group": [],
          "hide": false,
          "metricColumn": "Ip",
          "rawQuery": true,
          "rawSql": "SELECT\n  Tiempo AS \"time\",\n  Title AS metric,\n  NumErrDif\nFROM Canales INNER JOIN Programas ON Canales.Ip = Programas.Ip\nWHERE\n  Tipo = 0\nORDER BY Tiempo, Canales.id",
          "refId": "B",
          "select": [
            [
              {
                "params": [
                  "NumErrDif"
                ],
                "type": "column"
              }
            ]
          ],
          "table": "Canales",
          "timeColumn": "Tiempo",
          "timeColumnType": "double",
          "where": [
            {
              "name": "$__unixEpochFilter",
              "params": [],
              "type": "macro"
            },
            {
              "datatype": "int",
              "name": "",
              "params": [
                "Tipo",
                "=",
                "0"
              ],
              "type": "expression"
            }
          ]
        },
        {
          "format": "time_series",
          "group": [],
          "hide": true,
          "metricColumn": "Ip",
          "rawQuery": true,
          "rawSql": "SELECT\n  Tiempo AS \"time\",\n  Ip AS metric,\n  NumErrDif\nFROM Canales\nWHERE\n  Tipo = 1\nORDER BY Tiempo, id",
          "refId": "A",
          "select": [
            [
              {
                "params": [
                  "NumErrDif"
                ],
                "type": "column"
              }
            ]
          ],
          "table": "Canales",
          "timeColumn": "Tiempo",
          "timeColumnType": "double",
          "where": [
            {
              "name": "$__unixEpochFilter",
              "params": [],
              "type": "macro"
            },
            {
              "datatype": "int",
              "name": "",
              "params": [
                "Tipo",
                "=",
                "1"
              ],
              "type": "expression"
            }
          ]
        }
      ],
      "thresholds": [],
      "timeFrom": null,
      "timeRegions": [],
      "timeShift": null,
      "title": "Anomalías IGMP en 10 segundos",
      "tooltip": {
        "shared": true,
        "sort": 0,
        "value_type": "individual"
      },
      "type": "graph",
      "xaxis": {
        "buckets": null,
        "mode": "time",
        "name": null,
        "show": true,
        "values": []
      },
      "yaxes": [
        {
          "format": "short",
          "label": "Errores IGMP",
          "logBase": 1,
          "max": null,
          "min": null,
          "show": true
        },
        {
          "format": "short",
          "label": null,
          "logBase": 1,
          "max": null,
          "min": null,
          "show": false
        }
      ],
      "yaxis": {
        "align": false,
        "alignLevel": null
      }
    },
    {
      "aliasColors": {},
      "bars": false,
      "dashLength": 10,
      "dashes": false,
      "datasource": "MySQL",
      "description": "Paquetes capturados acumulativos y en intervalos de 10 segundos",
      "fill": 1,
      "fillGradient": 0,
      "gridPos": {
        "h": 8,
        "w": 12,
        "x": 0,
        "y": 24
      },
      "hiddenSeries": false,
      "id": 8,
      "legend": {
        "avg": false,
        "current": false,
        "max": false,
        "min": false,
        "show": true,
        "total": false,
        "values": false
      },
      "lines": true,
      "linewidth": 1,
      "nullPointMode": "null",
      "options": {
        "dataLinks": []
      },
      "percentage": false,
      "pointradius": 2,
      "points": false,
      "renderer": "flot",
      "seriesOverrides": [],
      "spaceLength": 10,
      "stack": false,
      "steppedLine": false,
      "targets": [
        {
          "format": "time_series",
          "group": [],
          "hide": true,
          "metricColumn": "Ip",
          "rawQuery": true,
          "rawSql": "SELECT\n  Tiempo AS \"time\",\n  Ip AS metric,\n  NumPaqDif\nFROM Canales\nWHERE\n  Tipo = 1\nORDER BY Tiempo, id",
          "refId": "A",
          "select": [
            [
              {
                "params": [
                  "NumPaqDif"
                ],
                "type": "column"
              }
            ]
          ],
          "table": "Canales",
          "timeColumn": "Tiempo",
          "timeColumnType": "double",
          "where": [
            {
              "name": "$__unixEpochFilter",
              "params": [],
              "type": "macro"
            },
            {
              "datatype": "int",
              "name": "",
              "params": [
                "Tipo",
                "=",
                "1"
              ],
              "type": "expression"
            }
          ]
        },
        {
          "format": "time_series",
          "group": [],
          "hide": false,
          "metricColumn": "Ip",
          "rawQuery": true,
          "rawSql": "SELECT\n  Tiempo AS \"time\",\n  Title AS metric,\n  NumPaqDif\nFROM Canales INNER JOIN Programas ON Canales.Ip = Programas.Ip\nWHERE\n  Tipo = 0\nORDER BY Tiempo,Canales.Id",
          "refId": "B",
          "select": [
            [
              {
                "params": [
                  "NumPaqDif"
                ],
                "type": "column"
              }
            ]
          ],
          "table": "Canales",
          "timeColumn": "Tiempo",
          "timeColumnType": "double",
          "where": [
            {
              "name": "$__unixEpochFilter",
              "params": [],
              "type": "macro"
            },
            {
              "datatype": "int",
              "name": "",
              "params": [
                "Tipo",
                "=",
                "0"
              ],
              "type": "expression"
            }
          ]
        }
      ],
      "thresholds": [],
      "timeFrom": null,
      "timeRegions": [],
      "timeShift": null,
      "title": "Paquetes capturados en 10 segundos",
      "tooltip": {
        "shared": true,
        "sort": 0,
        "value_type": "individual"
      },
      "type": "graph",
      "xaxis": {
        "buckets": null,
        "mode": "time",
        "name": null,
        "show": true,
        "values": []
      },
      "yaxes": [
        {
          "format": "short",
          "label": "Paquetes",
          "logBase": 1,
          "max": null,
          "min": null,
          "show": true
        },
        {
          "format": "short",
          "label": null,
          "logBase": 1,
          "max": null,
          "min": null,
          "show": false
        }
      ],
      "yaxis": {
        "align": false,
        "alignLevel": null
      }
    },
    {
      "aliasColors": {},
      "bars": false,
      "dashLength": 10,
      "dashes": false,
      "datasource": "MySQL",
      "description": "Tasa media de Mbit/s en intervalos de 10 segundos",
      "fill": 1,
      "fillGradient": 0,
      "gridPos": {
        "h": 8,
        "w": 12,
        "x": 12,
        "y": 24
      },
      "hiddenSeries": false,
      "id": 2,
      "legend": {
        "alignAsTable": false,
        "avg": false,
        "current": false,
        "max": false,
        "min": false,
        "rightSide": false,
        "show": true,
        "total": false,
        "values": false
      },
      "lines": true,
      "linewidth": 1,
      "nullPointMode": "null",
      "options": {
        "dataLinks": []
      },
      "percentage": false,
      "pointradius": 2,
      "points": false,
      "renderer": "flot",
      "seriesOverrides": [],
      "spaceLength": 10,
      "stack": false,
      "steppedLine": false,
      "targets": [
        {
          "format": "time_series",
          "group": [],
          "hide": false,
          "metricColumn": "Ip",
          "rawQuery": true,
          "rawSql": "SELECT\n  Tiempo AS \"time\",\n  Title AS metric,\n  Throughput\nFROM Canales INNER JOIN Programas ON Canales.Ip = Programas.Ip\nWHERE\n  Tipo = 0\nORDER BY Tiempo,Canales.id",
          "refId": "B",
          "select": [
            [
              {
                "params": [
                  "BytesDiff"
                ],
                "type": "column"
              }
            ]
          ],
          "table": "Canales",
          "timeColumn": "Tiempo",
          "timeColumnType": "double",
          "where": [
            {
              "name": "$__unixEpochFilter",
              "params": [],
              "type": "macro"
            },
            {
              "datatype": "int",
              "name": "",
              "params": [
                "Tipo",
                "=",
                "0"
              ],
              "type": "expression"
            }
          ]
        },
        {
          "format": "time_series",
          "group": [],
          "hide": true,
          "metricColumn": "Ip",
          "rawQuery": true,
          "rawSql": "SELECT\n  Tiempo AS \"time\",\n  Ip AS metric,\n  Throughput\nFROM Canales\nWHERE\n  Tipo = 1\nORDER BY Tiempo,id",
          "refId": "A",
          "select": [
            [
              {
                "params": [
                  "BytesDiff"
                ],
                "type": "column"
              }
            ]
          ],
          "table": "Canales",
          "timeColumn": "Tiempo",
          "timeColumnType": "double",
          "where": [
            {
              "name": "$__unixEpochFilter",
              "params": [],
              "type": "macro"
            },
            {
              "datatype": "int",
              "name": "",
              "params": [
                "Tipo",
                "=",
                "1"
              ],
              "type": "expression"
            }
          ]
        }
      ],
      "thresholds": [],
      "timeFrom": null,
      "timeRegions": [],
      "timeShift": null,
      "title": "Tasa media de Mbit/s en intervalos de 10 segundos",
      "tooltip": {
        "shared": true,
        "sort": 0,
        "value_type": "individual"
      },
      "type": "graph",
      "xaxis": {
        "buckets": null,
        "mode": "time",
        "name": null,
        "show": true,
        "values": []
      },
      "yaxes": [
        {
          "format": "short",
          "label": "Mbits/s",
          "logBase": 1,
          "max": null,
          "min": null,
          "show": true
        },
        {
          "format": "short",
          "label": null,
          "logBase": 1,
          "max": null,
          "min": null,
          "show": false
        }
      ],
      "yaxis": {
        "align": false,
        "alignLevel": null
      }
    },
    {
      "aliasColors": {},
      "bars": false,
      "dashLength": 10,
      "dashes": false,
      "datasource": "MySQL",
      "description": "Retardo acumulativo y en intervalos de 10 segundos",
      "fill": 1,
      "fillGradient": 0,
      "gridPos": {
        "h": 8,
        "w": 12,
        "x": 0,
        "y": 32
      },
      "hiddenSeries": false,
      "id": 12,
      "legend": {
        "avg": false,
        "current": false,
        "max": false,
        "min": false,
        "show": true,
        "total": false,
        "values": false
      },
      "lines": true,
      "linewidth": 1,
      "nullPointMode": "null",
      "options": {
        "dataLinks": []
      },
      "percentage": false,
      "pointradius": 2,
      "points": false,
      "renderer": "flot",
      "seriesOverrides": [],
      "spaceLength": 10,
      "stack": false,
      "steppedLine": false,
      "targets": [
        {
          "format": "time_series",
          "group": [],
          "hide": false,
          "metricColumn": "Ip",
          "rawQuery": true,
          "rawSql": "SELECT\n  Tiempo AS \"time\",\n  Title AS metric,\n  RetDif\nFROM Canales INNER JOIN Programas ON Canales.Ip = Programas.Ip \nWHERE\n  Tipo = 0\nORDER BY Tiempo,Canales.id",
          "refId": "B",
          "select": [
            [
              {
                "params": [
                  "RetDif"
                ],
                "type": "column"
              }
            ]
          ],
          "table": "Canales",
          "timeColumn": "Tiempo",
          "timeColumnType": "double",
          "where": [
            {
              "name": "$__unixEpochFilter",
              "params": [],
              "type": "macro"
            },
            {
              "datatype": "int",
              "name": "",
              "params": [
                "Tipo",
                "=",
                "0"
              ],
              "type": "expression"
            }
          ]
        },
        {
          "format": "time_series",
          "group": [],
          "hide": true,
          "metricColumn": "Ip",
          "rawQuery": true,
          "rawSql": "SELECT\n  Tiempo AS \"time\",\n  Ip AS metric,\n  RetDif\nFROM Canales\nWHERE\n  Tipo = 1\nORDER BY Tiempo,id",
          "refId": "A",
          "select": [
            [
              {
                "params": [
                  "RetDif"
                ],
                "type": "column"
              }
            ]
          ],
          "table": "Canales",
          "timeColumn": "Tiempo",
          "timeColumnType": "double",
          "where": [
            {
              "name": "$__unixEpochFilter",
              "params": [],
              "type": "macro"
            },
            {
              "datatype": "int",
              "name": "",
              "params": [
                "Tipo",
                "=",
                "1"
              ],
              "type": "expression"
            }
          ]
        }
      ],
      "thresholds": [],
      "timeFrom": null,
      "timeRegions": [],
      "timeShift": null,
      "title": "Tiempo entre llegadas en 10 segundos",
      "tooltip": {
        "shared": true,
        "sort": 0,
        "value_type": "individual"
      },
      "type": "graph",
      "xaxis": {
        "buckets": null,
        "mode": "time",
        "name": null,
        "show": true,
        "values": []
      },
      "yaxes": [
        {
          "format": "short",
          "label": "Microsegundos",
          "logBase": 1,
          "max": null,
          "min": null,
          "show": true
        },
        {
          "format": "short",
          "label": null,
          "logBase": 1,
          "max": null,
          "min": null,
          "show": false
        }
      ],
      "yaxis": {
        "align": false,
        "alignLevel": null
      }
    },
    {
      "aliasColors": {},
      "bars": false,
      "dashLength": 10,
      "dashes": false,
      "datasource": "MySQL",
      "description": "Otro tráfico UDP que existe en intervalos de 10 segundos",
      "fill": 1,
      "fillGradient": 0,
      "gridPos": {
        "h": 8,
        "w": 12,
        "x": 12,
        "y": 32
      },
      "hiddenSeries": false,
      "id": 20,
      "legend": {
        "avg": false,
        "current": false,
        "max": false,
        "min": false,
        "show": false,
        "total": false,
        "values": false
      },
      "lines": true,
      "linewidth": 1,
      "nullPointMode": "null",
      "options": {
        "dataLinks": []
      },
      "percentage": false,
      "pointradius": 2,
      "points": false,
      "renderer": "flot",
      "seriesOverrides": [],
      "spaceLength": 10,
      "stack": false,
      "steppedLine": false,
      "targets": [
        {
          "format": "time_series",
          "group": [],
          "metricColumn": "none",
          "rawQuery": true,
          "rawSql": "SELECT\n  Tiempo AS \"time\",\n  Num\nFROM Ruido\nORDER BY Tiempo",
          "refId": "A",
          "select": [
            [
              {
                "params": [
                  "Num"
                ],
                "type": "column"
              }
            ]
          ],
          "table": "Ruido",
          "timeColumn": "Tiempo",
          "timeColumnType": "double",
          "where": [
            {
              "name": "$__unixEpochFilter",
              "params": [],
              "type": "macro"
            }
          ]
        }
      ],
      "thresholds": [],
      "timeFrom": null,
      "timeRegions": [],
      "timeShift": null,
      "title": "Otro tráfico UDP cada 10 segundos",
      "tooltip": {
        "shared": true,
        "sort": 0,
        "value_type": "individual"
      },
      "type": "graph",
      "xaxis": {
        "buckets": null,
        "mode": "time",
        "name": null,
        "show": true,
        "values": []
      },
      "yaxes": [
        {
          "format": "short",
          "label": "Paquetes UDP",
          "logBase": 1,
          "max": null,
          "min": null,
          "show": true
        },
        {
          "format": "short",
          "label": null,
          "logBase": 1,
          "max": null,
          "min": null,
          "show": true
        }
      ],
      "yaxis": {
        "align": false,
        "alignLevel": null
      }
    }
  ],
  "refresh": false,
  "schemaVersion": 22,
  "style": "dark",
  "tags": [],
  "templating": {
    "list": []
  },
  "time": {
    "from": "2020-03-17T10:18:28.595Z",
    "to": "2020-03-17T10:25:25.094Z"
  },
  "timepicker": {
    "refresh_intervals": [
      "5s",
      "10s",
      "30s",
      "1m",
      "5m",
      "15m",
      "30m",
      "1h",
      "2h",
      "1d"
    ]
  },
  "timezone": "",
  "title": "Monitorización de calidad IPTV",
  "uid": "PXRiKP_Wk",
  "variables": {
    "list": []
  },
  "version": 97
}